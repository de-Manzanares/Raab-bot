module;

#include <array>
#include <locale>

import defs;

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
