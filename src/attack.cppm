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
 * @details The classic 0x88 square checking trick :-)
 * @param sq the square in question
 * @return true - is on board, false - is off board
 */
export bool is_valid_square(const Square sq) { return (sq & 0x88) == 0; }

/**
 * @param board the board in question
 * @param sq the square in question
 * @param by_color the attacking color
 * @return true - that square is attacked by by_color, that square is not
 * attacked by by_color
 */
export bool is_attacked(const Board &board, Square sq, Color by_color);

//------------------------------------------------------------------------------

bool is_attacked(const Board &board, const Square sq, const Color by_color) {

  // pawns
  if (by_color == white) {
    for (constexpr std::array dirs{SW, SE}; const auto dir : dirs) {
      if (is_valid_square(sq + dir) && board.piece_on[sq + dir] == 'P') {
        return true;
      }
    }
  }
  if (by_color == black) {
    for (constexpr std::array dirs{NW, NE}; const auto dir : dirs) {
      if (is_valid_square(sq + dir) && board.piece_on[sq + dir] == 'p') {
        return true;
      }
    }
  }

  // knights
  if (by_color == white) {
    for (const auto vec : vectors[knight]) {
      if (const Square square{sq + vec};
          is_valid_square(square) && board.piece_on[square] == 'N') {
        return true;
      }
    }
  }
  if (by_color == black) {
    for (const auto vec : vectors[knight]) {
      if (const Square square{sq + vec};
          is_valid_square(square) && board.piece_on[square] == 'n') {
        return true;
      }
    }
  }

  // diagonal
  if (by_color == white) {
    for (const auto vec : vectors[bishop]) {
      for (int i = 1;; ++i) {
        const Square square{sq + vec * i};
        if (!is_valid_square(square)) {
          break;
        }
        const auto piece = board.piece_on[square];
        if (piece == 'B' || piece == 'Q') {
          return true;
        }
        if (piece != '.') {
          break;
        }
      }
    }
  }
  if (by_color == black) {
    for (const auto vec : vectors[bishop]) {
      for (int i = 1;; ++i) {
        const Square square{sq + vec * i};
        if (!is_valid_square(square)) {
          break;
        }
        const auto piece = board.piece_on[square];
        if (piece == 'b' || piece == 'q') {
          return true;
        }
        if (piece != '.') {
          break;
        }
      }
    }
  }

  // vertical and horizontal
  if (by_color == white) {
    for (const auto vec : vectors[rook]) {
      for (int i = 1;; ++i) {
        const Square square{sq + vec * i};
        if (!is_valid_square(square)) {
          break;
        }
        const auto piece = board.piece_on[square];
        if (piece == 'R' || piece == 'Q') {
          return true;
        }
        if (piece != '.') {
          break;
        }
      }
    }
  }
  if (by_color == black) {
    for (const auto vec : vectors[rook]) {
      for (int i = 1;; ++i) {
        const Square square{sq + vec * i};
        if (!is_valid_square(square)) {
          break;
        }
        const auto piece = board.piece_on[square];
        if (piece == 'r' || piece == 'q') {
          return true;
        }
        if (piece != '.') {
          break;
        }
      }
    }
  }

  // kings
  if (by_color == white) {
    for (const auto vec : vectors[king]) {
      if (const Square square{sq + vec};
          is_valid_square(square) && board.piece_on[square] == 'K') {
        return true;
      }
    }
  }
  if (by_color == black) {
    for (const auto vec : vectors[king]) {
      if (const Square square{sq + vec};
          is_valid_square(square) && board.piece_on[square] == 'k') {
        return true;
      }
    }
  }

  return false;
}