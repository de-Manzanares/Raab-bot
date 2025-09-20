/**
 * @file fen.cppm
 * fen specific functions
 */

module;

#include <array>
#include <locale>

export module Board0x88:fen;
import :types;

//------------------------------------------------------------------------------

export namespace fen {

/// startpos FEN string
constexpr std::string_view startpos =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

/**
 * @param ch a character in a fen string
 * @return true if ch is a valid piece code, false otherwise
 */
bool is_piece(char ch);

/**
 * @param n n'th square in the fen sequence
 * @return corresponding index on a 0x88 board
 */
int to_0x88_idx(int n);

/**
 * @param ch fen piece code
 * @return corresponding PieceInfo
 * @example P-> {pawn, white}. k -> {king, black}
 * @note the inverse of get_fen_char_code
 */
PieceInfo get_piece_info(char ch);

/**
 * @param piece_info attributes of a given piece
 * @return the corresponding fen character code
 * @example {pawn, white} -> P. {king, black} -> k.
 * @note the inverse of get_piece_info
 */
char get_char_code(PieceInfo piece_info);

} // namespace fen

//------------------------------------------------------------------------------

bool fen::is_piece(const char ch) {
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
constexpr std::array<int, 64> fen_0x88_idx_sequence = {
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

int fen::to_0x88_idx(const int n) { return fen_0x88_idx_sequence[n]; }

PieceInfo fen::get_piece_info(char ch) {
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
  default:
  }

  if (std::islower(ch) != 0) {
    piece_info.color = black;
  } else {
    piece_info.color = white;
  }

  return piece_info;
}

char fen::get_char_code(const PieceInfo piece_info) {
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
  default:
  }

  if (piece_info.color == white) {
    ch = std::toupper(ch);
  }

  return ch;
}
