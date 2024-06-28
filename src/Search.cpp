#include "Search.h"
#include "Eval.h"

Node *Search::min_max(Node *n, uint depth, double alpha, double beta, // NOLINT
                      const bool maximizing) {
  if (depth == 0 || n->_child.empty()) {
    return n;
  }

  Node *opt_node = nullptr;

  if (maximizing) {
    double max = -100'000;
    for (const auto &c : n->_child) {
      const auto res = min_max(c, depth - 1, alpha, beta, false);
      if (max < res->_eval) {
        max = res->_eval;
        opt_node = res;
      }
      alpha = std::max(alpha, res->_eval);
      if (beta <= alpha) {
        break;
      }
    }
    return opt_node;
  }

  double min = 100'000;
  for (const auto &c : n->_child) {
    const auto res = min_max(c, depth - 1, alpha, beta, true);
    if (min > res->_eval) {
      min = res->_eval;
      opt_node = res;
    }
    beta = std::min(beta, res->_eval);
    if (beta <= alpha) {
      break;
    }
  }
  return opt_node;
}
