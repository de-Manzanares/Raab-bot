module;

#include <array>
#include <iostream>
#include <string_view>

import fen;
export module Board;

// clang-format off

enum class [[maybe_unused]] Square : int {
  a1 = 0x00, b1, c1, d1, e1, f1, g1, h1,
  a2 = 0x10, b2, c2, d2, e2, f2, g2, h2,
  a3 = 0x20, b3, c3, d3, e3, f3, g3, h3,
  a4 = 0x30, b4, c4, d4, e4, f4, g4, h4,
  a5 = 0x40, b5, c5, d5, e5, f5, g5, h5,
  a6 = 0x50, b6, c6, d6, e6, f6, g6, h6,
  a7 = 0x60, b7, c7, d7, e7, f7, g7, h7,
  a8 = 0x70, b8, c8, d8, e8, f8, g8, h8,
};

// clang-format on

/**
 * @class Board
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
  for (const auto ch : fen) {
    if (is_piece_letter(ch)) {
      piece_on[fen_square(i)] = ch;
      color_on[fen_square(i)] = color(ch);
      i++;
    } else if (ch >= '1' && ch <= '8') {
      i += ch - '0';
    } else if (ch == '/') { // do nothing
    } else if (ch == ' ') { // todo the rest of the game state stuff
      break;
    }
  }

  // minimal error checking
  // did we get all the squares?
  if (i != 64) {
    throw std::runtime_error("invalid FEN: wrong square count");
  }
}

void Board::display() {
  for (int i = 0; i < 64; ++i) {
    std::cout << ' ' << piece_on[fen_square(i)];
    if ((i + 1) % 8 == 0) {
      std::cout << '\n';
    }
  }
}
