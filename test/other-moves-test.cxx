#include <catch2/catch_all.hpp>
#include <iostream>
#include "../src/Board.hpp"

void generate_and_sort(Board& board, const Square& sq, std::vector<Square>& v)
{
    v.clear();
    board.update_move_maps();
    Color c = board.what_color(sq);
    if (c == Color::white) { v = board.move_map_white[sq]; }
    if (c == Color::black) { v = board.move_map_black[sq]; }
    std::sort(v.begin(), v.end());
}

using s = Square;

Board board;
std::vector<Square> v;

TEST_CASE("en passant")
{
    SECTION("single target white") {
        board.import_fen("4k3/8/8/8/2Pp4/8/8/4K3 w - - 0 1");                 // no target
        generate_and_sort(board, s::d4, v);
        CHECK(v == std::vector<Square>{s::d3});
        board.import_fen("4k3/8/8/8/2Pp4/8/8/4K3 w - c3 0 1");                // target
        generate_and_sort(board, s::d4, v);
        CHECK(v == std::vector<Square>{s::d3, s::c3});
    }SECTION("single target black") {
        board.import_fen("4k3/8/8/2Pp4/8/8/8/4K3 w - d6 0 1");
        generate_and_sort(board, s::c5, v);
        CHECK(v == std::vector<Square>{s::d6, s::c6});
    }SECTION("double target white") {
        board.import_fen("4k3/8/8/8/1pPp4/8/8/4K3 w - c3 0 1");
        generate_and_sort(board, s::d4, v);
        CHECK(v == std::vector<Square>{s::d3, s::c3});
        generate_and_sort(board, s::b4, v);
        CHECK(v == std::vector<Square>{s::c3, s::b3});
    }SECTION("double target black") {
        board.import_fen("4k3/8/8/2PpP3/8/8/8/4K3 w - d6 0 1");
        generate_and_sort(board, s::e5, v);
        CHECK(v == std::vector<Square>{s::e6, s::d6});
        generate_and_sort(board, s::c5, v);
        CHECK(v == std::vector<Square>{s::d6, s::c6});
    }SECTION("remove en passant pawn") {
        board.import_fen("8/8/2Pp4/6K1/8/8/2k5/8 w - d7 0 1");
        board.move(s::c6, s::d7, 0);
        CHECK(board.is_empty(Square::d6));
    }
}

TEST_CASE("pinned pieces rook")
{
    board.import_fen("8/8/8/1k1pR2K/8/8/8/8 w - - 0 1");
    CHECK(board.pinned_piece_rook(s::e5) == s::d5);
}
