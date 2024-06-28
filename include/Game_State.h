/*
 * Copyright (c) 2024 de-Manzanares
 * This work is released under the MIT license.
 */

#ifndef INCLUDE_GAME_STATE_H_
#define INCLUDE_GAME_STATE_H_

#include "Square.h"

#include <string>

using uint = unsigned int;

/**
 * @brief Represents the current state of a game.
 * @details  Stores information such as the active color, castling ability, en
 * passant targets, half move clock, and full move number.
 */
struct Game_State {
  /**
   * @brief Copy assignment operator
   * @param rhs The object to be copied
   * @return A reference to the lhs object after assignment
   */
  Game_State &operator=(const Game_State &rhs);

  /// @return The the active color: w or b.
  [[nodiscard]] char fen_active_color() const {
    return active_color == Color::white ? 'w' : 'b';
  }

  /**
   * @brief Get the castling ability.
   * @return Castling ability in FEN format.
   */
  [[nodiscard]] std::string fen_castling_ability() const;

  /**
   * @brief Get the en passant target.
   * @return En passant target in FEN format.
   */
  [[nodiscard]] std::string fen_en_passant_targets() const;

  /**
   * @brief Get the half move clock.
   * @return Half move clock in FEN format.
   */
  [[nodiscard]] std::string fen_half_move_clock() const;

  /**
   * @brief Get the full move number.
   * @return The full move number as a string.
   */
  [[nodiscard]] std::string fen_full_move_number() const;

  /**
   * @brief Clears the current game state.
   * @details set everything to 0/false/empty
   */
  void clear();

  // TODO AHah
  /**
   * @brief Set castling abilities
   * If the input string is "-", all castling abilities are set to false.
   * Otherwise, each character in the string represents a castling ability.
   * Valid characters are 'K', 'Q', 'k', and 'q'.
   * @param s The input string representing the castling ability.
   */
  void set_castling_ability(const std::string &s);

  // game-state data
  Color active_color = Color::white; ///< who's turn is it anyway?
  bool castle_w_K = true;            ///< white can castle king side
  bool castle_w_Q = true;            ///< white can castle queen side
  bool castle_b_k = true;            ///< black can castle king side
  bool castle_b_q = true;            ///< black can castle queen side
  std::string en_passant_target;     ///< en passant target
  bool en_passant_set = false; ///< was an en passant target created this turn?
  uint half_move_clock{};      ///< for the 50 move rule
  uint full_move_number = 1;   ///< starts at 1, increments after black moves
  bool black_inCheck = false;  ///< black king is in check
  bool white_inCheck = false;  ///< white king is in check
};

#endif // INCLUDE_GAME_STATE_H_
