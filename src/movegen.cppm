module;

#include <array>

import Attack;
import Board;
import chess.types;

export module movegen;

enum Flag { normal, capture, null_flag };

struct move {
  Square from = null_square;
  Square to = null_square;
  Flag flag = null_flag;
};

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

export std::array<move, 256> movegen(const Board &board);

// todo
void movegen_pawn() {}

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

std::array<move, 256> movegen(const Board &board) {
  int move_count = 0;
  std::array<move, 256> moves{};

  //  castling
  if (board.stm == white) {
    if (board.castling_rights & 1) { // white ks
      if ((board.piece_on[f1] == '.') && (board.piece_on[g1] == '.') &&
          !is_attacked(board, e1, black) && !is_attacked(board, f1, black) &&
          !is_attacked(board, g1, black)) {
        moves[move_count++] = {.from = e1, .to = g1, .flag = normal};
      }
    }
    if (board.castling_rights & 2) { // white qs
      if ((board.piece_on[c1] == '.') && (board.piece_on[d1] == '.') &&
          !is_attacked(board, c1, black) && !is_attacked(board, d1, black) &&
          !is_attacked(board, e1, black)) {
        moves[move_count++] = {.from = e1, .to = c1, .flag = normal};
      }
    }
  } else {
    if (board.castling_rights & 4) { // black ks
      if ((board.piece_on[f8] == '.') && (board.piece_on[g8] == '.') &&
          !is_attacked(board, e8, white) && !is_attacked(board, f8, white) &&
          !is_attacked(board, g8, white)) {
        moves[move_count++] = {.from = e8, .to = g8, .flag = normal};
      }
    }
    if (board.castling_rights & 8) { // black qs
      if ((board.piece_on[c8] == '.') && (board.piece_on[d8] == '.') &&
          !is_attacked(board, c8, white) && !is_attacked(board, d8, white) &&
          !is_attacked(board, e8, white)) {
        moves[move_count++] = {.from = e8, .to = c8, .flag = normal};
      }
    }
  }

  for (const auto from : square_sequence) {
    if (board.color_on[from] == board.stm) {
      auto piece_char = board.piece_on[from];
      if (piece_char == 'P' || piece_char == 'p') {
        movegen_pawn(); // special handling for pawns
      } else {
        const Piece piece = piece_type(piece_char);
        if (piece == knight || piece == king) {
          for (const auto vec : vectors[piece]) {
            Square to = from + vec;
            if (is_valid(to)) {
              if (board.color_on[to] == null_color) {
                moves[move_count++] = {.from = from, .to = to, .flag = normal};
              } else if (board.color_on[to] == ~board.stm) {
                moves[move_count++] = {.from = from, .to = to, .flag = capture};
              }
            }
          }
        } else if (piece != null_piece) {
          for (const auto vec : vectors[piece]) {
            for (int i = 1;; ++i) {
              Square to = from + (vec * i);
              if (!is_valid(to) || board.color_on[to] == board.stm) {
                break;
              }
              if (board.color_on[to] == null_color) {
                moves[move_count++] = {.from = from, .to = to, .flag = normal};
              } else if (board.color_on[to] == ~board.stm) {
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
