/**
 * @file Board.cppm
 * interface and implementation of the Board representation
 */

module;

#include <array>
#include <iostream>
#include <string_view>

import Color;
import fen;
export module Board;
/**
 * @class Board
 * @brief 0x88 board representation and associated functionality
 */
export class Board {
 public:
  Board(std::string_view fen);
  void display();

  std::array<char, 128> piece_on{};
  std::array<char, 128> color_on{}; // separate tracking of color
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
    throw std::runtime_error("invalid FEN: wrong square count");
  }
}

void Board::display() const {
  for (int i = 0; i < 64; ++i) {
    std::cout << ' ' << piece_on[fen_square(i)];
    if ((i + 1) % 8 == 0) {
      std::cout << '\n';
    }
  }
}
