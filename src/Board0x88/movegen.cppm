/**
 * @file movegen.cppm
 * move generators
 */

module;

#include <array>
#include <concepts>
#include <iostream>

export module Board0x88:movegen;
import :attack;
import :core;
import :move;
import :types;

template <class... Squares>
  requires(std::same_as<Square, Squares> && ...)
constexpr bool all_empty(const Board &b, Squares... sq) {
  return (... && (b.piece_on[sq] == null_piece));
}

template <class... Squares>
  requires(std::same_as<Square, Squares> && ...)
constexpr bool all_capturable(const Board &b, Squares... sq) {
  return (... && (b.color_on[sq] == ~b.stm));
}

template <class... Squares>
  requires(std::same_as<Square, Squares> && ...)
constexpr bool all_not_attacked(const Board &b, Color c, Squares... sq) {
  return (... && !is_attacked(b, sq, c));
}

constexpr int score_capture(const Piece victim, const Piece attacker) {
  return (10 * piece_value[victim]) - piece_value[attacker];
}

//------------------------------------------------------------------------------

/**
 * populate a container with pseudo-legal moves, return the count
 * @param b the board in question
 * @param out output iterator
 * @return number of pseudo-legal moves
 */
export template <class OutputIt>
  requires std::output_iterator<OutputIt, Move>
constexpr std::size_t movegen(const Board &b, OutputIt out);
// todo ^ history scoring

export constexpr std::size_t n_legal_moves(Board &b) {
  std::size_t legal_moves{};
  std::array<Move, 256> ml{};
  movegen(b, ml.begin());
  for (const auto m : ml) {
    if (m.from_square == null_square) {
      break;
    }
    move(b, m);
    // b.display();
    if (!in_check(b)) {
      ++legal_moves;
    }
    un_move(b, m);
    // b.display();
  }
  return legal_moves;
}

export constexpr std::size_t movegen_perft(Board &b, int depth) {
  std::size_t nodes{};
  std::array<Move, 256> ml{};
  if (depth == 1) {
    return n_legal_moves(b);
  }
  movegen(b, ml.begin());
  for (const auto m : ml) {
    if (m.from_square == null_square) {
      break;
    }
    move(b, m);
    if (!in_check(b)) {
      nodes += movegen_perft(b, depth - 1);
    }
    un_move(b, m);
  }
  return nodes;
}

//------------------------------------------------------------------------------

// clang-format off
constexpr std::array<Square, 64> square_sequence{
    { a1 , b1, c1, d1, e1, f1, g1, h1,
         a2 , b2, c2, d2, e2, f2, g2, h2,
         a3 , b3, c3, d3, e3, f3, g3, h3,
         a4 , b4, c4, d4, e4, f4, g4, h4,
         a5 , b5, c5, d5, e5, f5, g5, h5,
         a6 , b6, c6, d6, e6, f6, g6, h6,
         a7 , b7, c7, d7, e7, f7, g7, h7,
         a8 , b8, c8, d8, e8, f8, g8, h8,
    }};
// clang-format on

template <class OutputIt>
  requires std::output_iterator<OutputIt, Move>
constexpr std::size_t movegen_castle(const Board &b, OutputIt &out);

/// non-capture pawn moves
template <class OutputIt>
  requires std::output_iterator<OutputIt, Move>
constexpr std::size_t nc_pm(const Board &b, OutputIt &out, Square from);

/// capturing pawn moves
template <class OutputIt>
  requires std::output_iterator<OutputIt, Move>
constexpr std::size_t c_pm(const Board &b, OutputIt &out, Square from);

template <class OutputIt>
  requires std::output_iterator<OutputIt, Move>
constexpr std::size_t movegen_pawn(const Board &b, OutputIt &out, Square from) {
  return c_pm(b, out, from) + nc_pm(b, out, from);
}

template <class OutputIt>
  requires std::output_iterator<OutputIt, Move>
constexpr std::size_t movegen_not_pawn(const Board &b, OutputIt &out,
                                       Square from, Piece piece_t, int max_i);

//------------------------------------------------------------------------------

export template <class OutputIt>
  requires std::output_iterator<OutputIt, Move>
[[nodiscard]] constexpr std::size_t movegen(const Board &b, OutputIt out) {
  std::size_t move_count = 0;
  move_count += movegen_castle(b, out);
  for (const auto from : square_sequence) {
    if (b.color_on[from] == b.stm) {
      const auto [piece_t, color] = b.piece_info(from);
      if (piece_t == pawn) {
        move_count += movegen_pawn(b, out, from);
        continue;
      }
      int max_i{};
      piece_t == knight || piece_t == king ? max_i = 1 : max_i = 7;
      move_count += movegen_not_pawn(b, out, from, piece_t, max_i);
    }
  }
  return move_count;
}

//------------------------------------------------------------------------------

template <class OutputIt>
  requires std::output_iterator<OutputIt, Move>
constexpr std::size_t movegen_castle(const Board &b, OutputIt &out) {
  std::size_t move_count{};
  if (b.stm == white) {
    if ((b.castling_rights & 1) && all_empty(b, f1, g1) &&
        all_not_attacked(b, black, e1, f1, g1)) {
      *out++ = Move{
          .from_square = e1,
          .to_square = g1,
          .from_piece = {.piece_type = king, .color = white},
          .flag = castle,
      };
      ++move_count;
    }
    if ((b.castling_rights & 2) && all_empty(b, c1, d1) &&
        all_not_attacked(b, black, c1, d1, e1)) {
      *out++ = Move{
          .from_square = e1,
          .to_square = c1,
          .from_piece = {.piece_type = king, .color = white},
          .flag = castle,
      };
      ++move_count;
    }
  } else {
    if ((b.castling_rights & 4) && all_empty(b, f8, g8) &&
        all_not_attacked(b, white, e8, f8, g8)) {
      *out++ = Move{
          .from_square = e8,
          .to_square = g8,
          .from_piece = {.piece_type = king, .color = black},
          .flag = castle,
      };
      ++move_count;
    }
    if ((b.castling_rights & 8) && all_empty(b, c8, d8) &&
        all_not_attacked(b, white, c8, d8, e8)) {
      *out++ = Move{
          .from_square = e8,
          .to_square = c8,
          .from_piece = {.piece_type = king, .color = black},
          .flag = castle,
      };
      ++move_count;
    }
  }
  return move_count;
}

template <class OutputIt>
  requires std::output_iterator<OutputIt, Move>
constexpr std::size_t nc_pm(const Board &b, OutputIt &out, const Square from) {
  std::size_t move_count{};
  const Direction dir = b.stm == white ? N : S;
  const Square prom_row = b.stm == white ? a7 : a2;
  const Square double_row = b.stm == white ? a2 : a7;
  if (Square to{from + dir}; is_valid_square(to) && all_empty(b, to)) {
    if (from >> 4 == prom_row >> 4) { // if on 7th rank -> promotions
      for (constexpr std::array p_pieces = {queen, rook, bishop, knight};
           const auto p_piece : p_pieces) {
        *out++ = Move{
            .from_square = from,
            .to_square = to,
            .from_piece = {.piece_type = pawn, .color = b.stm},
            .flag = promotion,
            .promotion_piece = p_piece,
        };
        ++move_count;
      }
    } else {
      // single move
      *out++ = Move{
          .from_square = from,
          .to_square = to,
          .from_piece = {.piece_type = pawn, .color = b.stm},
          .flag = normal,
      };
      ++move_count;
      // double move
      if (from >> 4 == double_row >> 4 && all_empty(b, from + (2 * dir))) {
        to = from + (2 * dir);
        *out++ = Move{
            .from_square = from,
            .to_square = to,
            .from_piece = {.piece_type = pawn, .color = b.stm},
            .flag = en_passant,
            .ep_target = from + dir,
        };
        ++move_count;
        // todo ep target
        // b.ep = from + N; (?)
      }
    }
  }
  return move_count;
}

template <class OutputIt>
  requires std::output_iterator<OutputIt, Move>
constexpr std::size_t c_pm(const Board &b, OutputIt &out, const Square from) {
  std::size_t move_count{};
  std::array<Direction, 2> dirs;
  Square prom_row = b.stm == white ? a7 : a2;
  if (b.stm == white) {
    dirs = {NW, NE};
  } else {
    dirs = {SW, SE};
  }
  for (const auto dir : dirs) {
    const Square to = from + dir;
    if (is_valid_square(to)) {
      if (b.color_on[to] == ~b.stm) {
        // mvv/lva: most valuable victim, least valuable attacker
        const int score = score_capture(pawn, pawn);
        if ((from >> 4) == (prom_row >> 4)) {
          for (constexpr std::array p_pieces = {queen, rook, bishop, knight};
               const auto p_piece : p_pieces) {
            *out++ = Move{
                .from_square = from,
                .to_square = to,
                .from_piece = {.piece_type = pawn, .color = b.stm},
                .flag = prom_capture,
                .c_piece = b.piece_on[to],
                .promotion_piece = p_piece,
                .score = score + piece_value[p_piece],
            };
            ++move_count;
          }
        } else {
          *out++ = Move{
              .from_square = from,
              .to_square = to,
              .from_piece = {.piece_type = pawn, .color = b.stm},
              .flag = capture,
              .c_piece = b.piece_on[to],
              .score = score,
          };
          ++move_count;
        }
      } else if (to == b.ep) {
        *out++ = Move{
            .from_square = from,
            .to_square = to,
            .from_piece = {.piece_type = pawn, .color = b.stm},
            .flag = en_passant_capture,
            .c_piece = pawn,
            .score = score_capture(pawn, pawn),
        };
        ++move_count;
      }
    }
  }
  return move_count;
}

template <class OutputIt>
  requires std::output_iterator<OutputIt, Move>
constexpr std::size_t movegen_not_pawn(const Board &b, OutputIt &out,
                                       const Square from, const Piece piece_t,
                                       const int max_i) {
  std::size_t move_count{};
  for (const auto vec : vectors[piece_t]) {
    for (int i = 1; i <= max_i; ++i) {
      const Square to{from + (vec * i)};
      if (!is_valid_square(to) || b.color_on[to] == b.stm) {
        break;
      }
      if (all_empty(b, to)) {
        *out++ = Move{
            .from_square = from,
            .to_square = to,
            .from_piece = {.piece_type = piece_t, .color = b.stm},
            .flag = normal,
        };
        ++move_count;
      } else if (all_capturable(b, to)) {
        *out++ = Move{
            .from_square = from,
            .to_square = to,
            .from_piece = {.piece_type = piece_t, .color = b.stm},
            .flag = capture,
            .c_piece = b.piece_on[to],
            .score = score_capture(b.piece_on[to], b.piece_on[from]),
        };
        ++move_count;
        break;
      }
    }
  }
  return move_count;
}
