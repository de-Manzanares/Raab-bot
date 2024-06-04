#include <catch2/catch_all.hpp>
#include <iostream>
#include "../src/Board.hpp"
#include "../src/Tree.hpp"

TEST_CASE("creating a new node")
{
    Node start("6k1/1Q6/6K1/8/8/8/8/8 w - - 0 1");
    start.spawn(1);
    std::cout << start.count_nodes() << "\n";

    for (const auto& child : start._child) {
        std::cout << child->_move << " : " << child->_eval;
        char ch = child->_board.game_state.active_color == Color::white ? 'w' : 'b';
        std::cout << " " << ch << "\n";
    }

    std::vector<std::string> path = min_max(&start, 1);
    std::cout << "\n";
    for (const auto& move : path) {
        std::cout << move << " ";
    }
}

TEST_CASE("frustrated")
{
    Node n("1Q3k2/8/5K2/8/8/8/8/8 b - - 0 1");
    std::cout << n._eval;
}