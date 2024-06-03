#include <catch2/catch_all.hpp>
#include <iostream>
#include "../src/Board.hpp"

using s=Square;

Board board;

TEST_CASE("sample game")
{
    board.import_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    CHECK(board.nodes_at_depth_1(Color::white) == 20);
    board.move(s::d2,s::d4);
    CHECK(board.export_fen()=="rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq d3 0 1");
    CHECK(board.nodes_at_depth_1(Color::black) == 20);
    
}