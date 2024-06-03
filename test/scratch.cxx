#include <catch2/catch_all.hpp>
#include <iostream>
#include "../src/Board.hpp"

Board board;

TEST_CASE("simple position node count")
{
    board.import_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    CHECK(board.nodes_at_depth_1(Color::white) == 20);
}