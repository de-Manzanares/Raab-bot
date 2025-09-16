/**
 * @file fen.cppm
 * fen specific functions
 */

module;

#include <array>
#include <locale>

export module fen;

/// startpos FEN string
export constexpr std::string_view startpos =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

/**
 * @param ch a character in a fen string
 * @return true if ch is a valid piece code, false otherwise
 */
export auto is_piece_letter(char ch) -> bool;

/**
 * @param n n'th square in the fen sequence
 * @return corresponding index on a 0x88 board
 */
export auto fen_square(int n) -> int;

/**
 * @param ch piece code
 * @return 'b' if black, 'w' if white.
 * @warning requires ch to be a letter
 */
export auto color(char ch) -> char;

bool is_piece_letter(char ch) {
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
export constexpr std::array<int, 64> fen_square_sequence = {
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

int fen_square(const int n) { return fen_square_sequence[n]; }

char color(const char ch) {
  if (std::islower(ch)) {
    return 'b';
  }
  return 'w';
}
