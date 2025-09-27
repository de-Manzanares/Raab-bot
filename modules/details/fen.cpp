module;

#include <array>
#include <locale>

import types;

module fen;

bool fen::is_piece(const char ch)
{
  switch (std::tolower(ch)) {
  case 'p':
    return true;
  case 'b':
    return true;
  case 'n':
    return true;
  case 'r':
    return true;
  case 'k':
    return true;
  case 'q':
    return true;
  default:
    return false;
  }
}

// clang-format off
/**
 * @brief to easily iterate over the board while reading a fen string
 */
constexpr std::array<I8, 64> fen_0x88_idx_sequence = {
  {
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  }};
// clang-format on

I8 fen::to_0x88_idx(const unsigned long int n)
{
  return fen_0x88_idx_sequence[n];
}

PieceInfo fen::get_piece_info(char ch)
{
  PieceInfo piece_info;

  switch (std::tolower(ch)) {
  case 'p':
    piece_info.piece_t = pawn;
    break;
  case 'b':
    piece_info.piece_t = bishop;
    break;
  case 'n':
    piece_info.piece_t = knight;
    break;
  case 'r':
    piece_info.piece_t = rook;
    break;
  case 'k':
    piece_info.piece_t = king;
    break;
  case 'q':
    piece_info.piece_t = queen;
    break;
  default:;
  }

  if (std::islower(ch) != 0) {
    piece_info.color = black;
  }
  else {
    piece_info.color = white;
  }

  return piece_info;
}

char fen::get_char_code(const PieceInfo piece_info)
{
  char ch = '.';

  switch (piece_info.piece_t) {
  case pawn:
    ch = 'p';
    break;
  case bishop:
    ch = 'b';
    break;
  case knight:
    ch = 'n';
    break;
  case rook:
    ch = 'r';
    break;
  case king:
    ch = 'k';
    break;
  case queen:
    ch = 'q';
    break;
  default:;
  }

  if (piece_info.color == white) {
    ch = std::toupper(ch);
  }

  return ch;
}
