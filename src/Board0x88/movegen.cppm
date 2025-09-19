/**
 * @file movegen.cppm
 * move generators
 */

module;

#include <array>

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

//------------------------------------------------------------------------------

export std::array<Move, 256> movegen(const Board &b);

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

void movegen_castle(std::array<Move, 256> &moves, int &move_count,
                    const Board &b);

void movegen_pawn(std::array<Move, 256> &moves, int &move_count, const Board &b,
                  Square from);

void movegen_not_pawn(std::array<Move, 256> &moves, int &move_count,
                      const Board &b, Square from, Piece piece_t, int max_i);

std::array<Move, 256> movegen(const Board &b) {
  int move_count = 0;
  std::array<Move, 256> moves{};

  movegen_castle(moves, move_count, b);

  for (const auto from : square_sequence) {
    if (b.color_on[from] == b.stm) {
      const auto [piece_t, color] = b.piece_info(from);
      if (piece_t == pawn) {
        movegen_pawn(moves, move_count, b, from);
        continue;
      }
      int max_i{};
      piece_t == knight || piece_t == king ? max_i = 1 : max_i = 7;
      movegen_not_pawn(moves, move_count, b, from, piece_t, max_i);
    }
  }
  return moves;
}

void movegen_castle(std::array<Move, 256> &moves, int &move_count,
                    const Board &b) {
  if (b.stm == white) {
    if ((b.castling_rights & 1) && all_empty(b, f1, g1) &&
        all_not_attacked(b, black, e1, f1, g1)) {
      moves[move_count++] = {.from = e1, .to = g1, .flag = castle};
    }
    if ((b.castling_rights & 2) && all_empty(b, c1, d1) &&
        all_not_attacked(b, black, c1, d1, e1)) {
      moves[move_count++] = {.from = e1, .to = c1, .flag = castle};
    }
  } else {
    if ((b.castling_rights & 4) && all_empty(b, f8, g8) &&
        all_not_attacked(b, white, e8, f8, g8)) {
      moves[move_count++] = {.from = e8, .to = g8, .flag = castle};
    }
    if ((b.castling_rights & 8) && all_empty(b, c8, d8) &&
        all_not_attacked(b, white, c8, d8, e8)) {
      moves[move_count++] = {.from = e8, .to = c8, .flag = castle};
    }
  }
}

/// non-capture pawn moves
void nc_pm(std::array<Move, 256> &moves, int &move_count, const Board &b,
           Square from);

/// capturing pawn moves
void c_pm(std::array<Move, 256> &moves, int &move_count, const Board &b,
          Square from);

// todo promotions
void movegen_pawn(std::array<Move, 256> &moves, int &move_count, const Board &b,
                  const Square from) {
  // capturing pawn moves
  c_pm(moves, move_count, b, from);

  // non-capture pawn moves
  nc_pm(moves, move_count, b, from);
}

void nc_pm(std::array<Move, 256> &moves, int &move_count, const Board &b,
           const Square from) {
  const Direction dir = b.stm == white ? N : S;
  const Square prom_row = b.stm == white ? a7 : a2;
  const Square double_row = b.stm == white ? a2 : a7;

  if (Square to{from + dir}; all_empty(b, to)) {
    if (from >> 4 == prom_row >> 4) { // if on 7th rank -> promotions
      for (constexpr std::array pieces = {queen, rook, bishop, knight};
           const auto piece : pieces) {
        moves[move_count++] = {
            .from = from, .to = to, .flag = promotion, .p_piece = piece};
      }
    } else {
      // single move
      moves[move_count++] = {.from = from, .to = to, .flag = normal};
      // double move
      if (from >> 4 == double_row >> 4 && all_empty(b, from + (2 * dir))) {
        to = from + (2 * dir);
        moves[move_count++] = {.from = from, .to = to, .flag = en_passant};
        // todo ep target
        // b.ep = from + N; (?)
      }
    }
  }
}

void c_pm(std::array<Move, 256> &moves, int &move_count, const Board &b,
          const Square from) {
  std::array<Direction, 2> dirs;
  Square prom_row = b.stm == white ? a7 : a2;

  if (b.stm == white) {
    dirs = {NW, NE};
  } else {
    dirs = {SW, SE};
  }
  for (const auto dir : dirs) {
    if (const Square to = from + dir;
        is_valid_square(to) && b.color_on[to] == ~b.stm) {
      if ((from >> 4) == (prom_row >> 4)) {
        for (constexpr std::array p_pieces = {queen, rook, bishop, knight};
             const auto p_piece : p_pieces) {
          moves[move_count++] = {.from = from,
                                 .to = to,
                                 .flag = prom_capture,
                                 .c_piece = b.piece_on[to],
                                 .p_piece = p_piece};
        }
      } else {
        moves[move_count++] = {
            .from = from, .to = to, .flag = capture, .c_piece = b.piece_on[to]};
      }
    }
  }
}

void movegen_not_pawn(std::array<Move, 256> &moves, int &move_count,
                      const Board &b, Square from, Piece piece_t, int max_i) {
  for (const auto vec : vectors[piece_t]) {
    for (int i = 1; i <= max_i; ++i) {
      const Square to{from + (vec * i)};
      if (!is_valid_square(to) || b.color_on[to] == b.stm) {
        break;
      }
      if (all_empty(b, to)) {
        moves[move_count++] = {.from = from, .to = to, .flag = normal};
      } else if (all_capturable(b, to)) {
        moves[move_count++] = {
            .from = from, .to = to, .flag = capture, .c_piece = b.piece_on[to]};
        break;
      }
    }
  }
}
