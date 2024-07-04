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

#include "Search.h"
#include "Eval.h"

std::shared_ptr<Node> Search::min_max(const std::shared_ptr<Node> &n, // NOLINT
                                      const uint depth, double alpha,
                                      double beta, const bool maximizing) {
  if (depth == 0 || n->child().empty()) {
    return n;
  }

  std::shared_ptr<Node> opt_node = nullptr;

  if (maximizing) {
    double max = -100'000;
    for (const auto &c : n->child()) {
      const auto res = min_max(c, depth - 1, alpha, beta, false);
      if (max < res->eval()) {
        max = res->eval();
        opt_node = res;
      }
      alpha = std::max(alpha, res->eval());
      if (beta <= alpha) {
        break;
      }
    }
    return opt_node;
  }

  double min = 100'000;
  for (const auto &c : n->child()) {
    const auto res = min_max(c, depth - 1, alpha, beta, true);
    if (min > res->eval()) {
      min = res->eval();
      opt_node = res;
    }
    beta = std::min(beta, res->eval());
    if (beta <= alpha) {
      break;
    }
  }
  return opt_node;
}
