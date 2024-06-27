#include "../include/Search.h"

/**
 * @brief Find the optimal node in a decision tree.
 * @param n The current node in the search tree.
 * @param depth The remaining depth to explore in the search tree.
 * @param alpha The alpha value used in alpha-beta pruning.
 * @param beta The beta value used in alpha-beta pruning.
 * @param maximizing A boolean flag indicating whether the current node is a
 * maximizing node or not.
 * @return The optimal node found based on the evaluation and specified depth.
 * @note This function assumes that the tree has been populated with valid nodes
 * and evaluations.
 */
Node *Search::min_max(Node *n, uint depth, double alpha, double beta,
                      bool maximizing) {
  if (depth == 0 || n->_child.empty()) {
    return n;
  }

  Node *opt_node = nullptr;

  if (maximizing) {
    double max = -100'000;
    for (const auto &c : n->_child) {
      auto res = min_max(c, depth - 1, alpha, beta, false);
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
  } else {
    double min = 100'000;
    for (const auto &c : n->_child) {
      auto res = min_max(c, depth - 1, alpha, beta, true);
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
}
