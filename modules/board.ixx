/**
 * @file board.ixx
 * 0x88 board representation
 */

module;

#include <array>
#include <string_view>
#include <vector>

import fen;
import config;
import defs;
import history;

export module board;

namespace raab_bot {

//------------------------------------------------------------------------------

/**
 * @class Board
 * @brief 0x88 board representation and associated functionality
 */
export class Board : public History<config::history_heuristic> {
 public:
  // NOLINTNEXTLINE(*-use-equals-default)
  Board() : Board(fen::startpos) {} ///< default startpos

  /// set up the board with a fen string
  explicit Board(std::string_view fenstr);

  [[nodiscard]] PieceInfo piece_info(Square sq) const;

  void reset();         ///< reset to startpos
  void display() const; ///< print a simple visualization of the board

  // piece tracking

  std::array<Piece, 128> piece_on{}; ///< 0x88 board representation
  std::array<Color, 128> color_on{}; ///< redundant 0x88 color tracking

  // game state tracking

  Color   stm{};           ///< side to move
  Square  ep{};            ///< en passant target
  U8      cr = 0b0000;     /// castling rights \n bqs = 8, bks = 4, wqs = 2, wks = 1
  sz_t    hmc{};           ///< half move clock
  sz_t    fmc{};           ///< full move clock
  Square  wks{};           ///< white king square
  Square  bks{};           ///< black king square
  Phase   phase{};         ///< middle_game or end_game
  U64     t_hash{};        ///< hash for transposition table
  U64     m_hash{};        ///< hash for material table
  score_t mat_bal[2]{};    ///< incrementally updated material balance
  score_t mat_cnt[2][6]{}; ///< piece counts
  score_t pos_bal[2]{};    ///< incrementally updated psqt balance

  // todo maybe array instead?
  std::vector<U64> pos_stack{}; ///< position stack for three-fold repetition detection
};

//------------------------------------------------------------------------------

/**
 * @details The classic 0x88 square checking trick :-)
 * @param sq the square in question
 * @return true - is on board, false - is off board
 */
export bool is_on_board(const Square sq) { return (sq & 0x88) == 0; }

} // namespace raab_bot
