/**
 * @file board.ixx
 * 0x88 board representation
 */

module;

#include <array>
#include <string_view>

import fen;
import types;

export module board;

//------------------------------------------------------------------------------

/**
 * @class Board
 * @brief 0x88 board representation and associated functionality
 */
export class Board {
 public:
  Board() : Board(fen::startpos) {} ///< default startpos
  explicit Board(
      std::string_view fenstr); ///< set up the board with a fen string

  PieceInfo piece_info(Square sq) const;

  void reset();         ///< reset to startpos
  void display() const; ///< print a simple visualization of the board

  // piece tracking

  std::array<Piece, 128> piece_on{}; ///< 0x88 board representation
  std::array<Color, 128> color_on{}; ///< redundant 0x88 color tracking

  // game state tracking

  Color  stm{}; ///< side to move
  Square ep{};  ///< en passant target
  /// castling rights \n bqs = 8, bks = 4, wqs = 2, wks = 1
  U8   cr = 0b0000;
  sz_t hmc{}; ///< half move clock
  sz_t fmc{}; ///< full move clock

  Square wks{}; ///< white king square
  Square bks{}; ///< black king square

  U64 t_hash{}; ///< hash for transposition table
  U64 m_hash{}; ///< hash for material table
};

//------------------------------------------------------------------------------

/**
 * @details The classic 0x88 square checking trick :-)
 * @param sq the square in question
 * @return true - is on board, false - is off board
 */
export bool is_on_board(const Square sq) { return (sq & 0x88) == 0; }
