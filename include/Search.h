/*
 *     ____              __          __          __
 *    / __ \____ _____ _/ /_        / /_  ____  / /_
 *   / /_/ / __ `/ __ `/ __ \______/ __ \/ __ \/ __/
 *  / _, _/ /_/ / /_/ / /_/ /_____/ /_/ / /_/ / /_
 * /_/ |_|\__,_/\__,_/_.___/     /_.___/\____/\__/
 *
 * Copyright (c) 2024 de-Manzanares
 * This work is released under the MIT license.
 *
 */

#ifndef INCLUDE_SEARCH_H_
#define INCLUDE_SEARCH_H_

#include "Board.h"
#include "Node.h"

#include <memory>

/**
 * @struct Search
 * @brief min-max alpha-beta pruning search algorithm.
 */
struct Search {
  /**
   * @brief Find the optimal node in a decision tree.
   * @param n The current node in the search tree.
   * @param depth The remaining depth to explore in the search tree.
   * @param alpha The alpha value used in alpha-beta pruning.
   * @param beta The beta value used in alpha-beta pruning.
   * @param maximizing Indicates whether the current node is white or black
   * @return The optimal node found based on the evaluation and specified depth.
   * @note This function assumes that the tree has been populated with valid
   * nodes and evaluations.
   */
  static std::shared_ptr<Node> min_max(const std::shared_ptr<Node> &n,
                                       uint depth, double alpha, double beta,
                                       bool maximizing);
};

#endif // INCLUDE_SEARCH_H_
