/**
 * @file attack.cppm
 * is a square attacked?
 */

module;

#include <array>

import Board;
import chess.types;

export module attack;

//------------------------------------------------------------------------------

/**
 * @param board the board in question
 * @param sq the square in question
 * @param by_color the attacking color
 * @return true - that square is attacked by by_color, that square is not
 * attacked by by_color
 */
export bool is_attacked(const Board &board, Square sq, Color by_color);

//------------------------------------------------------------------------------

/// is attacked by pawn ?
bool ia_p(const Board &board, Square sq, Color by_color);

/// is attacked by knight || king ?
bool ia_n_k(const Board &board, Square sq, Color by_color, Piece piece);

/// is attacked by bishop || rook || queen ?
/// @warning needs to be called twice - once for bishop and rook each
bool ia_b_r_q(const Board &board, Square sq, Color by_color, Piece piece);

bool is_attacked(const Board &board, const Square sq, const Color by_color) {

  // pawns
  if (ia_p(board, sq, by_color)) {
    return true;
  }

  // knights and king
  if (ia_n_k(board, sq, by_color, knight) ||
      ia_n_k(board, sq, by_color, king)) {
    return true;
  }

  // diagonal, vertical, and horizontal (bishops, rooks, queens)
  if (ia_b_r_q(board, sq, by_color, bishop) ||
      ia_b_r_q(board, sq, by_color, rook)) {
    return true;
  }

  return false;
}

bool ia_p(const Board &board, const Square sq, const Color by_color) {
  std::array<Direction, 2> pawn_dirs;
  if (by_color == white) {
    pawn_dirs = {SW, SE};
  } else {
    pawn_dirs = {NW, NE};
  }
  for (const auto dir : pawn_dirs) {
    if (const Square from{sq + dir};
        is_valid_square(from) &&
        board.piece_info(from) == PieceInfo{pawn, by_color}) {
      return true;
    }
  }
  return false;
}

bool ia_n_k(const Board &board, const Square sq, const Color by_color,
            const Piece piece) {
  for (const auto vec : vectors[piece]) {
    if (const Square from{sq + vec};
        is_valid_square(from) &&
        board.piece_info(from) == PieceInfo{piece, by_color}) {
      return true;
    }
  }
  return false;
}

bool ia_b_r_q(const Board &board, const Square sq, const Color by_color,
              const Piece piece) {
  int n_dirs = 4;
  for (const auto vec : vectors[piece]) {
    for (int i = 1;; ++i) {
      const Square from{sq + vec * i};
      if (!is_valid_square(from)) {
        break;
      }
      const auto pi = board.piece_info(from);
      if (pi.piece_type == null_piece) {
        continue;
      }
      if (pi.color == ~by_color) {
        break;
      }
      if (pi == PieceInfo{piece, by_color} ||
          pi == PieceInfo{queen, by_color}) {
        return true;
      }
      if (pi.piece_type != null_piece) {
        break;
      }
    }
    if (--n_dirs == 0) {
      break;
    }
  }
  return false;
}
