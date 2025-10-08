/**
 * @file board.ixx
 * 0x88 board representation
 */

module;

#include <array>
#include <string_view>
#include <vector>

import Array;
import fen;
import config;
import defs;
import history;
import Vector;

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
  Board() : Board(fen::startpos) {}        ///< default startpos
  explicit Board(std::string_view fenstr); ///< set up the board with a fen string

  [[nodiscard]] PieceInfo piece_info(Square sq) const;

  void reset();         ///< reset to startpos
  void display() const; ///< print a simple visualization of the board

  Piece       &piece_on(const Square sq) { return piece_on_[sq]; }
  Color       &color_on(const Square sq) { return color_on_[sq]; }
  Color       &stm() { return stm_; }
  Square      &ep() { return ep_; }
  U8          &cr() { return cr_; }
  sz_t        &hmc() { return hmc_; }
  sz_t        &fmc() { return fmc_; }
  Square      &king_sq(const Color c) { return c == white ? wks_ : bks_; }
  Phase       &phase() { return phase_; }
  U64         &t_hash() { return t_hash_; }
  U64         &m_hash() { return m_hash_; }
  score_t     &mat_bal(const Color c) { return mat_bal_[c]; }
  score_t     &mat_cnt(const PieceInfo pi) { return mat_cnt_[pi.color][pi.piece_t]; }
  score_t     &pos_bal(const Color c) { return pos_bal_[c]; }
  Vector<U64> &rep_stack() { return rep_stack_; }

  [[nodiscard]] Piece   piece_on(const Square sq) const { return piece_on_[sq]; }
  [[nodiscard]] Color   color_on(const Square sq) const { return color_on_[sq]; }
  [[nodiscard]] Color   stm() const { return stm_; }
  [[nodiscard]] Square  ep() const { return ep_; }
  [[nodiscard]] U8      cr() const { return cr_; }
  [[nodiscard]] sz_t    hmc() const { return hmc_; }
  [[nodiscard]] sz_t    fmc() const { return fmc_; }
  [[nodiscard]] Square  king_sq(const Color c) const { return c == white ? wks_ : bks_; }
  [[nodiscard]] Phase   phase() const { return phase_; }
  [[nodiscard]] U64     t_hash() const { return t_hash_; }
  [[nodiscard]] U64     m_hash() const { return m_hash_; }
  [[nodiscard]] score_t mat_bal() const { return mat_bal_[white] - mat_bal_[black]; }
  [[nodiscard]] score_t mat_cnt(const PieceInfo pi) const { return mat_cnt_[pi.color][pi.piece_t]; }
  [[nodiscard]] score_t pos_bal() const { return pos_bal_[white] - pos_bal_[black]; }
  [[nodiscard]] auto    rep_stack() const -> const Vector<U64>    &{ return rep_stack_; }

 private:
  // piece tracking

  Array<Piece, 128> piece_on_{}; ///< 0x88 board representation
  Array<Color, 128> color_on_{}; ///< redundant 0x88 color tracking

  // game state tracking

  Color  stm_{};    ///< side to move
  Square ep_{};     ///< en passant target
  U8     cr_{};     /// castling rights \n bqs = 8, bks = 4, wqs = 2, wks = 1
  sz_t   hmc_{};    ///< half move clock
  sz_t   fmc_{};    ///< full move clock
  Square wks_{};    ///< white king square
  Square bks_{};    ///< black king square
  Phase  phase_{};  ///< middle_game or end_game
  U64    t_hash_{}; ///< hash for transposition table
  U64    m_hash_{}; ///< hash for material table

  // incrementally updated for eval

  Array<score_t, 2>      mat_bal_{}; ///< material balance
  ndArray<score_t, 2, 6> mat_cnt_{}; ///< piece count
  Array<score_t, 2>      pos_bal_{}; ///< psqt balance
  Vector<U64> rep_stack_{}; ///< stack of Zobrist hashes for three-fold repetition detection
};

//------------------------------------------------------------------------------

/**
 * @details The classic 0x88 square checking trick :-)
 * @param sq the square in question
 * @return true - is on board, false - is off board
 */
export bool is_on_board(const Square sq) { return (sq & 0x88) == 0; }

} // namespace raab_bot
