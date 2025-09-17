module;

export module chess.types;

export enum Color { white = 'w', black = 'b', null_color = '.' };

Color operator~(const Color color) {
  if (color == white) {
    return black;
  }
  return white;
}

export enum Direction : int { NW = 15, N, NE, E = -1, W = 1, SW = -17, S, SE };

export enum Piece : int {
  king,
  queen,
  rook,
  bishop,
  knight,
  pawn,
  null_piece = '.'
};

bool is_sliding[5] = {false, true, true, true, false};

export constexpr int n_vectors[5] = {8, 8, 4, 4, 8};
export constexpr int vectors[5][8] = {{SW, S, SE, E, W, NW, N, NE},
                                      {SW, S, SE, E, W, NW, N, NE},
                                      {S, E, W, N},
                                      {SW, SE, NW, NE},
                                      {-33, -31, -18, -14, 14, 18, 31, 33}};

// clang-format off

export enum Square : int {
  null_square = -1,
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

export Square operator+(const Square lhs, Direction rhs) {
  return static_cast<Square>(static_cast<int>(lhs) + static_cast<int>(rhs));
}

export Square operator+(const Square lhs, const int rhs) {
  return static_cast<Square>(static_cast<int>(lhs) + rhs);
}
