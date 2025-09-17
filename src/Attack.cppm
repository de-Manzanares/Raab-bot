module;

#include <array>

import Board;
import chess.types;

export module Attack;

export bool is_valid(const Square sq) { return (sq & 0x88) == 0; }

export bool is_attacked(const Board &board, const Square sq,
                        const Color by_color) {
  // possibly keep an influence map instead of recalculating every time....
  // it is an early break though, so it's not too bad I suppose

  // pawns
  if (by_color == white) {
    for (constexpr std::array dirs{SW, SE}; const auto dir : dirs) {
      if (is_valid(sq + dir) && board.piece_on[sq + dir] == 'P') {
        return true;
      }
    }
  }
  if (by_color == black) {
    for (constexpr std::array dirs{NW, NE}; const auto dir : dirs) {
      if (is_valid(sq + dir) && board.piece_on[sq + dir] == 'p') {
        return true;
      }
    }
  }

  // knights
  if (by_color == white) {
    for (const auto vec : vectors[knight]) {
      Square square{sq + vec};
      if (is_valid(square) && board.piece_on[square] == 'N') {
        return true;
      }
    }
  }
  if (by_color == black) {
    for (const auto vec : vectors[knight]) {
      Square square{sq + vec};
      if (is_valid(square) && board.piece_on[square] == 'n') {
        return true;
      }
    }
  }

  // diagonal
  if (by_color == white) {
    for (const auto vec : vectors[bishop]) {
      for (int i = 1;; ++i) {
        Square square{sq + (vec * i)};
        if (!is_valid(square)) {
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
        Square square{sq + (vec * i)};
        if (!is_valid(square)) {
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
        Square square{sq + (vec * i)};
        if (!is_valid(square)) {
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
        Square square{sq + (vec * i)};
        if (!is_valid(square)) {
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
      Square square{sq + vec};
      if (is_valid(square) && board.piece_on[square] == 'K') {
        return true;
      }
    }
  }
  if (by_color == black) {
    for (const auto vec : vectors[king]) {
      Square square{sq + vec};
      if (is_valid(square) && board.piece_on[square] == 'k') {
        return true;
      }
    }
  }

  return false;
}