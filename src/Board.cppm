module;

#include <array>
#include <iostream>
#include <string_view>

import fen;
export module Board;

// clang-format off

enum class [[maybe_unused]] Square : int {
  a0 = 0x00, a1 = 0x01, a2 = 0x02, a3 =  0x03, a4 =  0x04, a5 =  0x05, a6 =  0x06, a7 =  0x07,
  b0 = 0x10, b1 = 0x11, b2 = 0x12, b3 =  0x13, b4 =  0x14, b5 =  0x15, b6 =  0x16, b7 =  0x17,
  c0 = 0x20, c1 = 0x21, c2 = 0x22, c3 =  0x23, c4 =  0x24, c5 =  0x25, c6 =  0x26, c7 =  0x27,
  d0 = 0x30, d1 = 0x31, d2 = 0x32, d3 =  0x33, d4 =  0x34, d5 =  0x35, d6 =  0x36, d7 =  0x37,
  e0 = 0x40, e1 = 0x41, e2 = 0x42, e3 =  0x43, e4 =  0x44, e5 =  0x45, e6 =  0x46, e7 =  0x47,
  f0 = 0x50, f1 = 0x51, f2 = 0x52, f3 =  0x53, f4 =  0x54, f5 =  0x55, f6 =  0x56, f7 =  0x57,
  g0 = 0x60, g1 = 0x61, g2 = 0x62, g3 =  0x63, g4 =  0x64, g5 =  0x65, g6 =  0x66, g7 =  0x67,
  h0 = 0x70, h1 = 0x71, h2 = 0x72, h3 =  0x73, h4 =  0x74, h5 =  0x75, h6 =  0x76, h7 =  0x77,
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
