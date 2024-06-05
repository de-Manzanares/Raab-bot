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

std::pair<double, Node *> min_max(Node *n, uint depth, double alpha, double beta, bool maximizing)
{
    // n->_child.size() == 0 ??? maybe something else
    if (depth == 0 || n->_child.empty()) { return {eval(&n->_board), n}; }

    Node *opt_node = nullptr;

    if (maximizing) {
        double max = -std::numeric_limits<double>::infinity();
        for (const auto& c : n->_child) {
            auto res = min_max(c, depth - 1, alpha, beta, false);
            if (max < res.first) {
                max = res.first;
                opt_node = res.second;
            }
            alpha = std::max(alpha, res.first);
            if (beta <= alpha) { break; }
        }
        return {max, opt_node};
    }
    else {
        double min = std::numeric_limits<double>::infinity();
        for (const auto& c : n->_child) {
            auto res = min_max(c, depth - 1, alpha, beta, true);
            if (min > res.first) {
                min = res.first;
                opt_node = res.second;
            }
            beta = std::min(beta, res.first);
            if (beta <= alpha) { break; }
        }
        return {min, opt_node};
    }
}

#endif //CHESSENGINE_SEARCH_HPP
