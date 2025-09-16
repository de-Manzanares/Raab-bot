/**
 * @file Board0x88.cppm
 * interface and implementation of the Board representation
 */

module;

#include <array>
#include <iostream>
#include <string_view>

import Color;
import fen;
import Piece;
import Square;

export module Board;
/**
 * @class Board
 * @brief 0x88 board representation and associated functionality
 */
export class Board {
 public:
  Board() : Board(startpos) {} ///< default startpos
  Board(std::string_view fen); ///< set up the board with a fen string

  void display() const; ///< print a simple visualization of the board

  // piece tracking

  std::array<char, 128> piece_on{}; ///< 0x88 board representation
  std::array<char, 128> color_on{}; ///< redundant 0x88 color tracking

  // game state tracking

  Color stm; ///< side to move
  Square ep; ///< en passant square

  /** @brief bitfield representing castling rights \n
   * [bqs, bks, wqs, wks]
   */
  int castling_rights = 0b0000;

  int hmc; ///< half move clock
  int fmc; ///< full move clock
};

Board::Board(std::string_view fen) {
  // iterate over squares
  piece_on.fill('.');
  color_on.fill('.');
  int i = 0;
  auto ch = fen.begin();
  for (; i < 64; ++ch) {
    if (is_piece_letter(*ch)) {
      piece_on[fen_square(i)] = *ch;
      color_on[fen_square(i)] = color(*ch);
      i++;
    } else if (*ch >= '1' && *ch <= '8') {
      i += *ch - '0';
    } else if (*ch == '/') { // do nothing
    } else if (*ch == ' ') {
      break;
    }
  }

  // minimal error checking
  // did we get all the squares?
  if (i != 64) {
    throw std::runtime_error("malformed FEN");
  }

  // side to move
  ++ch;
  *ch == 'w' ? stm = Color::white : stm = Color::black;
  std::advance(ch, 2);

  // castling rights
  if (*ch == '-') {
    castling_rights = 0;
    ++ch;
  } else {
    for (; ch != fen.end() && *ch != ' '; ++ch) {
      switch (*ch) {
      case 'K':
        castling_rights += 0b0001;
        break;
      case 'Q':
        castling_rights += 0b0010;
        break;
      case 'k':
        castling_rights += 0b0100;
        break;
      case 'q':
        castling_rights += 0b1000;
        break;
      }
    }
  }

  // en passant target
  ++ch;
  if (*ch == '-') {
    ep = Square::null;
    std::advance(ch, 2);
  } else {
    ep = static_cast<Square>(16 * (*std::next(ch) - '0' - 1) + *ch - 'a');
    std::advance(ch, 3);
  }

  // half-move clock, full-move clock
  hmc = *ch - '0';
  fmc = *std::next(ch, 2) - '0';
}

void Board::display() const {
  for (int i = 0; i < 64; ++i) {
    std::cout << ' ' << piece_on[fen_square(i)];
    if ((i + 1) % 8 == 0) {
      std::cout << '\n';
    }
  }
}
