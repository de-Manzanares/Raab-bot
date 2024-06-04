#include <catch2/catch_all.hpp>
#include <iostream>
#include "../src/Board.hpp"

using s=Square;

Board board;
std::vector<int> number_of_legal_moves = {
        0,
        20,20
        28,22,
        30,23,
        33,31,
        34,33,
        34,39,
        32,36,
        33,27,
        40,29,
        48,29
};
int i = 1;
TEST_CASE("sample game")
{
    board.import_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    CHECK(board.nodes_at_depth_1(Color::white) == 20);
    board.move(s::d2,s::d4);
    CHECK(board.export_fen()=="rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq d3 0 1");
    CHECK(board.nodes_at_depth_1(Color::black) == 20);
    board.move(s::g8,s::f6);
    CHECK(board.export_fen()=="rnbqkb1r/pppppppp/5n2/8/3P4/8/PPP1PPPP/RNBQKBNR w KQkq - 1 2");
    CHECK(board.nodes_at_depth_1(Color::white) == 28);
    board.move(s::c2,s::c4);
    CHECK(board.export_fen()=="rnbqkb1r/pppppppp/5n2/8/2PP4/8/PP2PPPP/RNBQKBNR b KQkq c3 0 2");
    CHECK(board.nodes_at_depth_1(Color::black) == 22);
    board.print_move_map(Color::black);
}