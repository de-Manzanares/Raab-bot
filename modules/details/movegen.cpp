module;

#include <algorithm>
#include <array>
#include <concepts>

import board;
import move;
import transposition;
import types;
import attack;

module movegen;

template <class... Squares>
  requires(std::same_as<Square, Squares> && ...)
bool all_empty(const Board &b, Squares... sq)
{
  return (... && (b.piece_on[sq] == null_piece));
}

template <class... Squares>
  requires(std::same_as<Square, Squares> && ...)
bool all_capturable(const Board &b, Squares... sq)
{
  return (... && (b.color_on[sq] == ~b.stm));
}

template <class... Squares>
  requires(std::same_as<Square, Squares> && ...)
bool all_not_attacked(const Board &b, Color c, Squares... sq)
{
  return (... && !is_attacked(b, sq, c));
}

constexpr U8 p_vals[6] = {9, 9, 5, 3, 3, 1}; //

score_t mvvlva(const Piece victim, const Piece attacker)
{
  return (10 * p_vals[victim]) - p_vals[attacker];
}

sz_t movegen_castle(const Board &b, MlIt &out);

/// capturing pawn moves
sz_t c_pm(const Board &b, MlIt &out, Square from);

/// non-capture pawn moves
sz_t nc_pm(const Board &b, MlIt &out, Square from);

sz_t movegen_pawn(const Board &b, MlIt &out, const Square from)
{
  return c_pm(b, out, from) + nc_pm(b, out, from);
}

sz_t movegen_not_pawn(const Board &b, MlIt &out, Square from, Piece piece_t,
                      int max_i);

//------------------------------------------------------------------------------
sz_t movegen(const Board &b, MlIt out)
{
  sz_t move_count = 0;
  move_count += movegen_castle(b, out);
  for (const auto from : square_sequence) {
    if (b.color_on[from] == b.stm) {
      const auto [piece_t, color] = b.piece_info(from);
      if (piece_t == pawn) {
        move_count += movegen_pawn(b, out, from);
        continue;
      }
      sz_t max_i{};
      piece_t == knight || piece_t == king ? max_i = 1 : max_i = 7;
      move_count += movegen_not_pawn(b, out, from, piece_t, max_i);
    }
  }
  return move_count;
}

sz_t quiescence_movegen(const Board &b, MlIt out)
{
  sz_t move_count = 0;
  for (const auto from : square_sequence) {
    if (b.color_on[from] == b.stm) {
      const auto [piece_t, color] = b.piece_info(from);
      if (piece_t == pawn) {
        move_count += c_pm(b, out, from);
        continue;
      }
      int max_i{};
      piece_t == knight || piece_t == king ? max_i = 1 : max_i = 7;
      for (const auto vec : unit_vectors[piece_t]) {
        for (int i = 1; i <= max_i; ++i) {
          const Square to{from + (vec * i)};
          if (!is_on_board(to) || b.color_on[to] == b.stm) {
            break;
          }
          if (all_capturable(b, to)) {
            *out++ = Move{
                .from_sq    = from,
                .to_sq      = to,
                .from_piece = {.piece_t = piece_t, .color = b.stm},
                .flag       = capture,
                .cap_piece  = b.piece_on[to],
                .score      = mvvlva(b.piece_on[to], b.piece_on[from]),
                .prev_cr    = b.cr,
                .prev_ep    = b.ep,
                .prev_hmc   = b.hmc,
            };
            ++move_count;
            break;
          }
        }
      }
    }
  }
  return move_count;
}

bool is_legal(const Board &b)
{
  return !is_attacked(b, b.stm == white ? b.bks : b.wks, b.stm);
}

sz_t cnt_legal_moves(Board &b)
{
  MoveList   ml;
  sz_t       cnt_legal_moves{};
  const auto sz = movegen(b, ml.begin());
  for (sz_t i = 0; i < sz; ++i) {
    move(b, ml[i]);
    if (is_legal(b)) {
      ++cnt_legal_moves;
    }
    unmove(b, ml[i]);
  }
  return cnt_legal_moves;
}

void movegen_sort(MlIt first, sz_t sz, const TT_move tt_m)
{
  if (tt_m != TT_move{}) {
    auto is_move = [&tt_m](const Move &m) {
      return (m.from_sq == tt_m.from && m.to_sq == tt_m.to &&
              m.prom_p == tt_m.prom_p);
    };
    auto it = std::find_if(first, std::next(first, sz), is_move);
    if (it != std::next(first, sz)) {
      std::iter_swap(first, it);
    }
  }
  else {
    auto max = first;
    for (auto it = std::next(first); it != std::next(first, sz); ++it) {
      if (it->score > max->score) {
        max = it;
      }
    }
    std::iter_swap(first, max);
  }
}

//------------------------------------------------------------------------------

sz_t movegen_castle(const Board &b, MlIt &out)
{
  sz_t move_count{};
  if (b.stm == white) {
    if ((b.cr & 1) && all_empty(b, f1, g1) &&
        all_not_attacked(b, black, e1, f1, g1)) {
      *out++ = Move{
          .from_sq    = e1,
          .to_sq      = g1,
          .from_piece = {.piece_t = king, .color = white},
          .flag       = castle,
          .prev_cr    = b.cr,
          .prev_ep    = b.ep,
          .prev_hmc   = b.hmc,
      };
      ++move_count;
    }
    if ((b.cr & 2) && all_empty(b, b1, c1, d1) &&
        all_not_attacked(b, black, c1, d1, e1)) {
      *out++ = Move{
          .from_sq    = e1,
          .to_sq      = c1,
          .from_piece = {.piece_t = king, .color = white},
          .flag       = castle,
          .prev_cr    = b.cr,
          .prev_ep    = b.ep,
          .prev_hmc   = b.hmc,
      };
      ++move_count;
    }
  }
  else {
    if ((b.cr & 4) && all_empty(b, f8, g8) &&
        all_not_attacked(b, white, e8, f8, g8)) {
      *out++ = Move{
          .from_sq    = e8,
          .to_sq      = g8,
          .from_piece = {.piece_t = king, .color = black},
          .flag       = castle,
          .prev_cr    = b.cr,
          .prev_ep    = b.ep,
          .prev_hmc   = b.hmc,
      };
      ++move_count;
    }
    if ((b.cr & 8) && all_empty(b, b8, c8, d8) &&
        all_not_attacked(b, white, c8, d8, e8)) {
      *out++ = Move{
          .from_sq    = e8,
          .to_sq      = c8,
          .from_piece = {.piece_t = king, .color = black},
          .flag       = castle,
          .prev_cr    = b.cr,
          .prev_ep    = b.ep,
          .prev_hmc   = b.hmc,
      };
      ++move_count;
    }
  }
  return move_count;
}

sz_t c_pm(const Board &b, MlIt &out, const Square from)
{
  sz_t                     move_count{};
  std::array<Direction, 2> dirs;
  const Square             prom_row = b.stm == white ? a7 : a2;
  if (b.stm == white) {
    dirs = {NW, NE};
  }
  else {
    dirs = {SW, SE};
  }
  for (const auto dir : dirs) {
    if (const Square to = from + dir; is_on_board(to)) {
      if (b.color_on[to] == ~b.stm) {
        const score_t score = mvvlva(b.piece_on[to], pawn);
        if ((from >> 4) == (prom_row >> 4)) {
          for (constexpr std::array p_pieces = {queen, rook, bishop, knight};
               const auto           p_piece : p_pieces) {
            *out++ = Move{
                .from_sq    = from,
                .to_sq      = to,
                .from_piece = {.piece_t = pawn, .color = b.stm},
                .flag       = prom_capture,
                .cap_piece  = b.piece_on[to],
                .prom_p     = p_piece,
                .score      = score + (16 * p_vals[p_piece]),
                .prev_cr    = b.cr,
                .prev_ep    = b.ep,
                .prev_hmc   = b.hmc,
            };
            ++move_count;
          }
        }
        else {
          *out++ = Move{
              .from_sq    = from,
              .to_sq      = to,
              .from_piece = {.piece_t = pawn, .color = b.stm},
              .flag       = capture,
              .cap_piece  = b.piece_on[to],
              .score      = score,
              .prev_cr    = b.cr,
              .prev_ep    = b.ep,
              .prev_hmc   = b.hmc,
          };
          ++move_count;
        }
      }
      else if (to == b.ep) {
        *out++ = Move{
            .from_sq    = from,
            .to_sq      = to,
            .from_piece = {.piece_t = pawn, .color = b.stm},
            .flag       = en_passant_capture,
            .cap_piece  = pawn,
            .score      = mvvlva(pawn, pawn),
            .prev_cr    = b.cr,
            .prev_ep    = b.ep,
            .prev_hmc   = b.hmc,
        };
        ++move_count;
      }
    }
  }
  return move_count;
}

sz_t nc_pm(const Board &b, MlIt &out, const Square from)
{
  sz_t            move_count{};
  const Direction dir        = b.stm == white ? N : S;
  const Square    prom_row   = b.stm == white ? a7 : a2;
  const Square    double_row = b.stm == white ? a2 : a7;
  if (Square to{from + dir}; is_on_board(to) && all_empty(b, to)) {
    if (from >> 4 == prom_row >> 4) { // if on 7th rank -> promotions
      for (constexpr std::array p_pieces = {queen, rook, bishop, knight};
           const auto           p_piece : p_pieces) {
        *out++ = Move{
            .from_sq    = from,
            .to_sq      = to,
            .from_piece = {.piece_t = pawn, .color = b.stm},
            .flag       = promotion,
            .prom_p     = p_piece,
            .score      = 16 * p_vals[p_piece],
            .prev_cr    = b.cr,
            .prev_ep    = b.ep,
            .prev_hmc   = b.hmc,
        };
        ++move_count;
      }
    }
    else {
      // single move
      *out++ = Move{
          .from_sq    = from,
          .to_sq      = to,
          .from_piece = {.piece_t = pawn, .color = b.stm},
          .flag       = normal,
          .prev_cr    = b.cr,
          .prev_ep    = b.ep,
          .prev_hmc   = b.hmc,
      };
      ++move_count;
      // double move
      if (from >> 4 == double_row >> 4 && all_empty(b, from + (2 * dir))) {
        to     = from + (2 * dir);
        *out++ = Move{
            .from_sq    = from,
            .to_sq      = to,
            .from_piece = {.piece_t = pawn, .color = b.stm},
            .flag       = double_push,
            .ep_target  = from + dir,
            .prev_cr    = b.cr,
            .prev_ep    = b.ep,
            .prev_hmc   = b.hmc,
        };
        ++move_count;
      }
    }
  }
  return move_count;
}

sz_t movegen_not_pawn(const Board &b, MlIt &out, const Square from,
                      const Piece piece_t, const int max_i)
{
  sz_t move_count{};
  for (const auto vec : unit_vectors[piece_t]) {
    for (int i = 1; i <= max_i; ++i) {
      const Square to{from + (vec * i)};
      if (!is_on_board(to) || b.color_on[to] == b.stm) {
        break;
      }
      if (all_empty(b, to)) {
        *out++ = Move{
            .from_sq    = from,
            .to_sq      = to,
            .from_piece = {.piece_t = piece_t, .color = b.stm},
            .flag       = normal,
            .prev_cr    = b.cr,
            .prev_ep    = b.ep,
            .prev_hmc   = b.hmc,
        };
        ++move_count;
      }
      else if (all_capturable(b, to)) {
        *out++ = Move{
            .from_sq    = from,
            .to_sq      = to,
            .from_piece = {.piece_t = piece_t, .color = b.stm},
            .flag       = capture,
            .cap_piece  = b.piece_on[to],
            .score      = mvvlva(b.piece_on[to], b.piece_on[from]),
            .prev_cr    = b.cr,
            .prev_ep    = b.ep,
            .prev_hmc   = b.hmc,
        };
        ++move_count;
        break;
      }
    }
  }
  return move_count;
}
