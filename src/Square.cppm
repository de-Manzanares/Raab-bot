/**
 * @file Square.cppm
 * Square type
 */

module;

import Piece;

export module Square;

// clang-format off

export enum Square : int {
  null = -1,
  a1 = 0x00, b1, c1, d1, e1, f1, g1, h1,
  a2 = 0x10, b2, c2, d2, e2, f2, g2, h2,
  a3 = 0x20, b3, c3, d3, e3, f3, g3, h3,
  a4 = 0x30, b4, c4, d4, e4, f4, g4, h4,
  a5 = 0x40, b5, c5, d5, e5, f5, g5, h5,
  a6 = 0x50, b6, c6, d6, e6, f6, g6, h6,
  a7 = 0x60, b7, c7, d7, e7, f7, g7, h7,
  a8 = 0x70, b8, c8, d8, e8, f8, g8, h8,
};

export Square operator+(Square lhs, Direction rhs) {
  return static_cast<Square>(static_cast<int>(lhs) + static_cast<int>(rhs));
}

// clang-format on
