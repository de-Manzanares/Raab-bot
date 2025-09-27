/**
 * @file types.ixx
 * enums, aliases, structs, and operators for more expressive code
 */

module;

#include <array>
#include <cstdint>

export module types;

//------------------------------------------------------------------------------

export using I8  = std::int8_t;
export using I16 = std::int16_t;
export using I32 = std::int32_t;
export using I64 = std::int64_t;

export using U8  = std::uint8_t;
export using U16 = std::uint16_t;
export using U32 = std::uint32_t;
export using U64 = std::uint64_t;

export using score_t = int;

export using sz_t = std::size_t;

//------------------------------------------------------------------------------

export enum Color : U8 { white, black, null_color = '.' };

export Color operator~(Color color);

///  0x88 specific movement values
export enum Direction : I8 { NW = 15, N, NE, E = 1, W = -1, SW = -17, S, SE };

/// piece codes to easily access movement types by piece type
/// see the vectors[5][8] array below
export enum Piece : U8 {
  king,
  queen,
  rook,
  bishop,
  knight,
  pawn,
  null_piece = '.'
};

export struct PieceInfo {
  Piece piece_t{null_piece};
  Color color{null_color};
};

export bool operator==(PieceInfo lhs, PieceInfo rhs);

/// the movement directions of each piece, excluding pawns
export constexpr I8 unit_vectors[5][8] = {{SW, S, SE, E, W, NW, N, NE},
                                          {SW, S, SE, E, W, NW, N, NE},
                                          {S, E, W, N},
                                          {SW, SE, NW, NE},
                                          {-33, -31, -18, -14, 14, 18, 31, 33}};

// clang-format off
/**
 * 0x88 specific square indices
 */
export enum Square : I8 {
  null_square = -1,
  a1 = 0x00U, b1, c1, d1, e1, f1, g1, h1,
  a2 = 0x10U, b2, c2, d2, e2, f2, g2, h2,
  a3 = 0x20U, b3, c3, d3, e3, f3, g3, h3,
  a4 = 0x30U, b4, c4, d4, e4, f4, g4, h4,
  a5 = 0x40U, b5, c5, d5, e5, f5, g5, h5,
  a6 = 0x50U, b6, c6, d6, e6, f6, g6, h6,
  a7 = 0x60U, b7, c7, d7, e7, f7, g7, h7,
  a8 = 0x70U, b8, c8, d8, e8, f8, g8, h8,
};
// clang-format on

export Square operator+(Square lhs, Direction rhs);
export Square operator+(Square lhs, int rhs);

// clang-format off
export constexpr std::array<Square, 64> square_sequence{
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

//------------------------------------------------------------------------------

bool operator==(const PieceInfo lhs, const PieceInfo rhs)
{
  return lhs.piece_t == rhs.piece_t && lhs.color == rhs.color;
}

Square operator+(const Square lhs, const Direction rhs)
{
  return static_cast<Square>(static_cast<int>(lhs) + static_cast<int>(rhs));
}

Square operator+(const Square lhs, const int rhs)
{
  return static_cast<Square>(static_cast<int>(lhs) + rhs);
}

Color operator~(const Color color)
{
  if (color == white) {
    return black;
  }
  return white;
}
