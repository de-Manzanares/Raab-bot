/*
 *     ____              __          __          __
 *    / __ \____ _____ _/ /_        / /_  ____  / /_
 *   / /_/ / __ `/ __ `/ __ \______/ __ \/ __ \/ __/
 *  / _, _/ /_/ / /_/ / /_/ /_____/ /_/ / /_/ / /_
 * /_/ |_|\__,_/\__,_/_.___/     /_.___/\____/\__/
 *
 * Copyright (c) 2024 de-Manzanares
 * This work is released under the MIT license.
 */

#ifndef INCLUDE_EVAL_H_
#define INCLUDE_EVAL_H_

#include "Board.h"
#include "Node.h"

#include <unordered_map>

struct Node;

/**
 * @struct Eval
 * @brief Evaluates a position
 */
struct Eval {
  static std::unordered_map<char, int> material_value; ///< material value

  // evaluation parameters
  static double CHECK_BONUS;         ///< weight of check
  static double MOBILITY_MULTIPLIER; ///< weight of count of legal moves
  static double CASTLE_BONUS;        ///< weight on castle
  static double RATIO_MULTIPLIER;    ///< weight on material ratio

  // evaluation functions ----------------------------------------

  /**
   * @brief Computes balance of material on the board.
   * @param n The node containing the board to evaluate.
   * @return The material balance.
   */
  static double material_evaluation(const Node *n);

  /**
   * @brief Detects stalemate or checkmate.
   * @details Check whether there are any legal moves available for the active
   * player. If there are no legal moves available and the player's king is not
   * in check, this is stalemate. If there are no legal moves available and the
   * player's king is in check, this is checkmate.
   * @param n The node containing the board to evaluate.
   * @return \n
   *         - -1: white is in checkmate \n
   *         -  0: stalemate
   *         -  1: black is in checkmate \n
   *         -  2: neither stalemate nor checkmate
   */
  static int detect_stalemate_checkmate(const Node *n);

  /**
   * @brief The number of legal moves available to a side.
   * @details Adds score for each legal move available to a side.
   * @param n The node containing the board to evaluate.
   * @return The mobility score.
   */
  static double mobility_evaluation(const Node *n);

  /**
   * @brief Gives weight to check
   * @param n The node containing the board to evaluate.
   * @return (+) - black is in check \n (-) - white is in check
   */
  static double check_bonus(const Node *n);

  /**
   * Encourage castling lol
   * I just wanted to see castling more often so I tacked some weight onto it.
   */
  static double castle_bonus(const Node *n);

  /**
   * Creates an aggregate score using all evaluation methods
   */
  static double simple_evaluation(const Node *n);

  /**
   * wrapper for simple_evaluation()
   */
  static double eval(const Node *n);

  // unused
  /**
   * @brief Calculates the material ratio for a given chess board.
   * @details intended to encourage trading when up on material, and discourage
   * trading when down on material
   * @param board Pointer to the Board object representing the chess board.
   * @return The ratio of material advantage to total material.
   */
  static double material_ratio(const Board *board);

  /**
   * In the first ten moves of the game, penalize queen movements, encourage
   * other piece movements
   * @note is not needed when an opening book is in use
   */
  static double discourage_early_queen_movement(const Node *node);
};

#endif // INCLUDE_EVAL_H_
