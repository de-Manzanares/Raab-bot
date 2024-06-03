#include <catch2/catch_all.hpp>
#include <iostream>
#include "../src/Board.hpp"
#include "../src/Tree.hpp"

TEST_CASE("creating a new node")
{
    Node start("8/2P5/1K6/8/5k2/8/8/8 w - - 0 1");
    start.spawn(2);
    std::cout << start.count_nodes();
}