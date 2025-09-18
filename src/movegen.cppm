/**
 * @file movegen.cppm
 * move generators
 */

module;

#include <array>

import attack;
import Board;
import chess.types;

export module movegen;

template <class... Squares>
  requires(std::same_as<Square, Squares> && ...)
constexpr bool all_empty(const Board &b, Squares... sq) {
  return (... && (b.piece_on[sq] == null_piece));
}

template <class... Squares>
  requires(std::same_as<Square, Squares> && ...)
constexpr bool all_not_attacked(const Board &b, Color c, Squares... sq) {
  return (... && !is_attacked(b, sq, c));
}

Piece piece_type(const char ch);

export enum Flag { normal, capture, en_passant, castle, promotion, null_flag };

export struct Move {
  Square from = null_square;
  Square to = null_square;
  Flag flag = null_flag;
  Piece promotion = null_piece;
};

export bool operator==(Move lhs, Move rhs) {
  return lhs.from == rhs.from && lhs.to == rhs.to && lhs.flag == rhs.flag;
}

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

// todo fix interface(?)
export std::array<Move, 256> movegen(const Board &b);

// todo promotions
void movegen_pawn(std::array<Move, 256> &moves, int &move_count,
                  const Board &board, Square from) {
  // pawn moves
  if (board.stm == white) {
    Square to = from + N;
    if (is_valid_square(to) && board.piece_on[to] == '.') {
      moves[move_count++] = {.from = from, .to = to, .flag = normal};
    }
    if ((from >> 4) == a2) { // on second rank
      to = from + (2 * N);
      if (is_valid_square(to) && board.piece_on[to] == '.') {
        moves[move_count++] = {.from = from, .to = to, .flag = en_passant};
        // todo ep target
        // board.ep = from + N;
      }
    }
  } else {
    Square to = from + S;
    if (is_valid_square(to) && board.piece_on[to] == '.') {
      moves[move_count++] = {.from = from, .to = to, .flag = normal};
    }
    if ((from >> 4) == a7) { // on seventh rank
      to = from + (2 * S);
      if (is_valid_square(to) && board.piece_on[to] == '.') {
        moves[move_count++] = {.from = from, .to = to, .flag = en_passant};
        // todo ep target
        // board.ep = from + S;
      }
    }
  }

  // pawn captures
  if (board.stm == white) {
    for (constexpr std::array dirs{NW, NE}; const auto dir : dirs) {
      if (const Square to = from + dir;
          is_valid_square(to) && board.color_on[to] == ~board.stm) {
        moves[move_count++] = {.from = from, .to = to, .flag = capture};
      }
    }
  } else {
    for (constexpr std::array dirs{SW, SE}; const auto dir : dirs) {
      if (const Square to = from + dir;
          is_valid_square(to) && board.color_on[to] == ~board.stm) {
        moves[move_count++] = {.from = from, .to = to, .flag = capture};
      }
    }
  }
}

std::array<Move, 256> movegen(const Board &b) {
  int move_count = 0;
  std::array<Move, 256> moves{};

  //  castling
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

  for (const auto from : square_sequence) {
    if (b.color_on[from] == b.stm) {
      auto pi = b.piece_info(from);
      if (pi.piece_type == pawn) {
        movegen_pawn(moves, move_count, b, from);
      } else {
        const Piece piece = pi.piece_type; // todo fix lol
        if (piece == knight || piece == king) {
          for (const auto vec : vectors[piece]) {
            Square to = from + vec;
            if (is_valid_square(to)) {
              if (b.color_on[to] == null_color) {
                moves[move_count++] = {.from = from, .to = to, .flag = normal};
              } else if (b.color_on[to] == ~b.stm) {
                moves[move_count++] = {.from = from, .to = to, .flag = capture};
              }
            }
          }
        } else if (piece != null_piece) {
          for (const auto vec : vectors[piece]) {
            for (int i = 1;; ++i) {
              Square to = from + (vec * i);
              if (!is_valid_square(to) || b.color_on[to] == b.stm) {
                break;
              }
              if (b.color_on[to] == null_color) {
                moves[move_count++] = {.from = from, .to = to, .flag = normal};
              } else if (b.color_on[to] == ~b.stm) {
                moves[move_count++] = {.from = from, .to = to, .flag = capture};
                break;
              }
            }
          }
        }
      }
    }
  }
  return moves;
}

// todo simplify movegen to take advantage of fixed board representation
Piece piece_type(const char ch) {
  switch (ch) {
  case 'P':
  case 'p':
    return pawn;
  case 'N':
  case 'n':
    return knight;
  case 'B':
  case 'b':
    return bishop;
  case 'R':
  case 'r':
    return rook;
  case 'Q':
  case 'q':
    return queen;
  case 'K':
  case 'k':
    return king;
  default: {
    return null_piece;
  }
  }
}
