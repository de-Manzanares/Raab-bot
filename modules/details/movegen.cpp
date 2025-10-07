module;

#include <algorithm>
#include <array>
#include <concepts>
#include <span>

import board;
import defs;
import move;
import transposition;
import attack;

module movegen;

namespace raab_bot {

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

MlIt movegen_castle(const Board &b, MlIt out);

/// capturing pawn moves
MlIt c_pm(const Board &b, MlIt out, Square from);

/// non-capture pawn moves
MlIt nc_pm(const Board &b, MlIt out, Square from);

MlIt movegen_pawn(const Board &b, const MlIt out, const Square from)
{
  return nc_pm(b, c_pm(b, out, from), from); // disgusting lol
}

MlIt movegen_not_pawn(const Board &b, MlIt out, Square from, Piece piece_t, int max_i);

//------------------------------------------------------------------------------

std::span<Move> movegen(const Board &b, const MlIt out)
{
  auto end = out;
  end = movegen_castle(b, end);
  for (const auto from : square_sequence) {
    if (b.color_on[from] == b.stm) {
      const auto [piece_t, color] = b.piece_info(from);
      if (piece_t == pawn) {
        end = movegen_pawn(b, end, from);
        continue;
      }
      sz_t max_i{};
      piece_t == knight || piece_t == king ? max_i = 1 : max_i = 7;
      end = movegen_not_pawn(b, end, from, piece_t, max_i);
    }
  }
  return {out, end};
}

std::span<Move> quiescence_movegen(const Board &b, MlIt out)
{
  auto end = out;
  for (const auto from : square_sequence) {
    if (b.color_on[from] == b.stm) {
      const auto [piece_t, color] = b.piece_info(from);
      if (piece_t == pawn) {
        end = c_pm(b, end, from);
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
            *end++ = Move{
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
            break;
          }
        }
      }
    }
  }
  return {out, end};
}

bool is_legal(const Board &b) { return !is_attacked(b, b.stm == white ? b.bks : b.wks, b.stm); }

sz_t cnt_legal_moves(Board &b)
{
  MoveList buf;
  sz_t     cnt{};
  for (const auto ml = movegen(b, buf.begin()); const auto &m : ml) {
    move(b, m);
    if (is_legal(b)) {
      ++cnt;
    }
    unmove(b, m);
  }
  return cnt;
}

void move_select(std::span<Move> ml, const TT_move tt_m)
{
  // probe the TT
  if (tt_m != TT_move{}) {
    auto is_move = [&tt_m](const Move &m) {
      return (m.from_sq == tt_m.from && m.to_sq == tt_m.to && m.prom_p == tt_m.prom_p);
    };
    if (const auto it = std::ranges::find_if(ml, is_move); it != ml.end()) {
      std::iter_swap(ml.begin(), it);
    }
  }
  else {
    auto max = ml.begin();
    for (auto it = std::next(max); it != ml.end(); ++it) {
      if (it->score > max->score) {
        max = it;
      }
    }
    std::iter_swap(ml.begin(), max);
  }
}

//------------------------------------------------------------------------------

MlIt movegen_castle(const Board &b, MlIt out)
{
  auto castle_move = [&b](const Square to) {
    const auto ksq = b.stm == white ? e1 : e8;
    return Move{
        .from_sq    = ksq,
        .to_sq      = to,
        .from_piece = {.piece_t = king, .color = b.stm},
        .flag       = castle,
        .score      = b.get_history(b.stm, ksq, to),
        .prev_cr    = b.cr,
        .prev_ep    = b.ep,
        .prev_hmc   = b.hmc,
    };
  };

  if (b.stm == white) {
    if ((b.cr & 1) && all_empty(b, f1, g1) && all_not_attacked(b, black, e1, f1, g1)) {
      *out++ = castle_move(g1);
    }
    if ((b.cr & 2) && all_empty(b, b1, c1, d1) && all_not_attacked(b, black, c1, d1, e1)) {
      *out++ = castle_move(c1);
    }
  }
  else {
    if ((b.cr & 4) && all_empty(b, f8, g8) && all_not_attacked(b, white, e8, f8, g8)) {
      *out++ = castle_move(g8);
    }
    if ((b.cr & 8) && all_empty(b, b8, c8, d8) && all_not_attacked(b, white, c8, d8, e8)) {
      *out++ = castle_move(c8);
    }
  }
  return out;
}

MlIt c_pm(const Board &b, MlIt out, const Square from)
{
  auto capture_pawn_move = [&b, from](const Square to, const Flag flag, const score_t score,
                                      const Piece prom_piece = null_piece) {
    return Move{
        .from_sq    = from,
        .to_sq      = to,
        .from_piece = {.piece_t = pawn, .color = b.stm},
        .flag       = flag,
        .cap_piece  = flag == en_passant_capture ? pawn : b.piece_on[to],
        .prom_p     = prom_piece,
        .score      = score,
        .prev_cr    = b.cr,
        .prev_ep    = b.ep,
        .prev_hmc   = b.hmc,
    };
  };

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
        const score_t cap_score = mvvlva(b.piece_on[to], pawn);

        if ((from >> 4) == (prom_row >> 4)) {
          for (constexpr std::array prom_pieces = {queen, rook, bishop, knight};
               const auto           prom_piece : prom_pieces) {
            *out++ = capture_pawn_move(to, prom_capture, hmax + cap_score + piece_val[prom_piece],
                                       prom_piece);
          }
        }
        else {
          *out++ = capture_pawn_move(to, capture, hmax + cap_score);
        }
      }
      else if (to == b.ep) {
        *out++ = capture_pawn_move(to, en_passant_capture, hmax + mvvlva(pawn, pawn));
      }
    }
  }
  return out;
}

MlIt nc_pm(const Board &b, MlIt out, const Square from)
{
  constexpr auto no_ep   = null_square;

  auto non_capture_pawn_move = [&b, from](const Square to, const Flag flag, const score_t score,
                                          const Square ep_target  = null_square,
                                          const Piece  prom_piece = null_piece) {
    return Move{
        .from_sq    = from,
        .to_sq      = to,
        .from_piece = {.piece_t = pawn, .color = b.stm},
        .flag       = flag,
        .prom_p     = prom_piece,
        .ep_target  = ep_target,
        .score      = score,
        .prev_cr    = b.cr,
        .prev_ep    = b.ep,
        .prev_hmc   = b.hmc,
    };
  };

  const Direction dir        = b.stm == white ? N : S;
  const Square    prom_row   = b.stm == white ? a7 : a2;
  const Square    double_row = b.stm == white ? a2 : a7;
  if (Square to{from + dir}; is_on_board(to) && all_empty(b, to)) {
    if (from >> 4 == prom_row >> 4) { // if on 7th rank -> promotions
      for (constexpr std::array prom_pieces = {queen, rook, bishop, knight};
           const auto           prom_piece : prom_pieces) {
        *out++ =
            non_capture_pawn_move(to, promotion, hmax + piece_val[prom_piece], no_ep, prom_piece);
      }
    }
    else {
      *out++ = non_capture_pawn_move(to, normal, b.get_history(b.stm, from, to));
      if (from >> 4 == double_row >> 4 && all_empty(b, from + (2 * dir))) {
        const auto ep_target = from + dir;
        to                   = from + (2 * dir);
        *out++ = non_capture_pawn_move(to, double_push, b.get_history(b.stm, from, to), ep_target);
      }
    }
  }
  return out;
}

MlIt movegen_not_pawn(const Board &b, MlIt out, const Square from, const Piece piece_t,
                      const int max_i)
{
  auto not_pawn_move = [&b, from, piece_t](const Square to, const Flag flag, const score_t score,
                                           const Piece cap_piece = null_piece) {
    return Move{
        .from_sq    = from,
        .to_sq      = to,
        .from_piece = {.piece_t = piece_t, .color = b.stm},
        .flag       = flag,
        .cap_piece  = cap_piece,
        .score      = score,
        .prev_cr    = b.cr,
        .prev_ep    = b.ep,
        .prev_hmc   = b.hmc,
    };
  };

  for (const auto vec : unit_vectors[piece_t]) {
    for (int i = 1; i <= max_i; ++i) {
      const Square to{from + (vec * i)};
      if (!is_on_board(to) || b.color_on[to] == b.stm) {
        break;
      }
      if (all_empty(b, to)) {
        *out++ = not_pawn_move(to, normal, b.get_history(b.stm, from, to));
      }
      else if (all_capturable(b, to)) {
        const auto score = hmax + mvvlva(b.piece_on[to], b.piece_on[from]);
        *out++           = not_pawn_move(to, capture, score, b.piece_on[to]);
        break;
      }
    }
  }
  return out;
}

} // namespace raab_bot
