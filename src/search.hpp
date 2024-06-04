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

double min_max(Node *n, uint depth, double alpha, double beta, bool maximizing)
{
    // n->_child.size() == 0 ??? maybe something else
    if (depth == 0 || n->_child.empty()) { return eval(&n->_board); }

    if (maximizing) {
        double max = -std::numeric_limits<double>::infinity();
        for (const auto& c : n->_child) {
            double temp_eval = min_max(c, depth - 1, alpha, beta, false);
            max = std::max(max, temp_eval);
            alpha = std::max(alpha, temp_eval);
            if (beta <= alpha) { break; }
        }
        return max;
    }
    else {
        double min = std::numeric_limits<double>::infinity();
        for (const auto& c : n->_child) {
            double temp_eval = min_max(c, depth - 1, alpha, beta, true);
            min = std::min(min, temp_eval);
            beta = std::min(beta, temp_eval);
            if (beta <= alpha) { break; }
        }
        return min;
    }
}

#endif //CHESSENGINE_SEARCH_HPP
