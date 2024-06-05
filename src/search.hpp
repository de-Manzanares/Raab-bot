#ifndef CHESSENGINE_SEARCH_HPP
#define CHESSENGINE_SEARCH_HPP

#include "Board.hpp"
#include "tree.hpp"

bool is_maximizing(Color current_color, uint at_depth)
{
    Color down_there = at_depth % 2 == 0 ? current_color : !current_color;
    return down_there == Color::white;
}

// TODO interleave tree generation and searching somehow

Node *min_max(Node *n, uint depth, double alpha, double beta, bool maximizing)
{
    // n->_child.size() == 0 ??? maybe something else
    if (depth == 0 || n->_child.empty()) { return n; }

    Node *opt_node = nullptr;

    if (maximizing) {
        double max = -std::numeric_limits<double>::infinity();
        for (const auto& c : n->_child) {
            auto res = min_max(c, depth - 1, alpha, beta, false);
            if (max < res->_eval) {
                max = res->_eval;
                opt_node = res;
            }
            alpha = std::max(alpha, res->_eval);
            if (beta <= alpha) { break; }
        }
        return opt_node;
    }
    else {
        double min = std::numeric_limits<double>::infinity();
        for (const auto& c : n->_child) {
            auto res = min_max(c, depth - 1, alpha, beta, true);
            if (min > res->_eval) {
                min = res->_eval;
                opt_node = res;
            }
            beta = std::min(beta, res->_eval);
            if (beta <= alpha) { break; }
        }
        return opt_node;
    }
}

#endif //CHESSENGINE_SEARCH_HPP
