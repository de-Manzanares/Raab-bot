/**
 * @file fen.ixx
 * fen specific functions
 */

module;

#include <string_view>

import types;

export module fen;

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
I8 to_0x88_idx(unsigned long int n);

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
