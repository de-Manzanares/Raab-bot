#include <catch2/catch_all.hpp>
#include <iostream>
#include "../src/Board.hpp"
#include "../src/FEN.hpp"
#include "../src/Move-and-rules.hpp"

Board board;
using s = Square;
using c = Color;

TEST_CASE("vertical up")
{
    import_fen(&board, "8/2k1q1r1/8/8/8/8/2K1Q1R1/8 w - - 0 1");
    CHECK(board.is_under_attack(s::c1, c::black) == true);
    CHECK(board.is_under_attack(s::c1, c::white) == false);
    CHECK(board.is_under_attack(s::e1, c::black) == true);
    CHECK(board.is_under_attack(s::e1, c::white) == false);
    CHECK(board.is_under_attack(s::g1, c::black) == true);
    CHECK(board.is_under_attack(s::g1, c::white) == false);
    CHECK(board.is_under_attack(s::e5, c::white) == true);
    CHECK(board.is_under_attack(s::g3, c::white) == true);
    import_fen(&board, "8/2q1q1q1/8/2b1n1p1/4K3/2K3K1/8/8 w - - 0 1");
    CHECK(board.is_under_attack(s::c3, board.what_color(s::c3)) == false);
    CHECK(board.is_under_attack(s::e4, board.what_color(s::e4)) == false);
    CHECK(board.is_under_attack(s::g3, board.what_color(s::g3)) == false);
    import_fen(&board, "4q3/8/8/8/8/8/8/1K2K3 w - - 0 1");
    CHECK(board.is_under_attack(s::b1, board.what_color(s::b1)) == false);
    CHECK(board.is_under_attack(s::e1, board.what_color(s::e1)) == true);
}

TEST_CASE("vertical down")
{
    import_fen(&board, "KKKKKKKK/8/8/5p2/4n3/3b4/2r5/1q6 w - - 0 1");
    CHECK(board.is_under_attack(s::a8, board.what_color(s::a8)) == false);
    CHECK(board.is_under_attack(s::b8, board.what_color(s::b8)) == true);
    CHECK(board.is_under_attack(s::c8, board.what_color(s::c8)) == true);
    CHECK(board.is_under_attack(s::d8, board.what_color(s::d8)) == false);
    CHECK(board.is_under_attack(s::e8, board.what_color(s::e8)) == false);
    CHECK(board.is_under_attack(s::f8, board.what_color(s::f8)) == false);
    import_fen(&board, "KKKKKKKK/8/8/5p2/4n3/1PPb4/2r5/1q6 w - - 0 1");
    CHECK(board.is_under_attack(s::b8, board.what_color(s::b8)) == false);
    CHECK(board.is_under_attack(s::c8, board.what_color(s::c8)) == false);
}

TEST_CASE("horizontal left")
{
    import_fen(&board, "8/k1K5/q2K4/r3K3/b4K2/n5K1/p6K/7K w - - 0 1");
    CHECK(board.is_under_attack(s::c7, board.what_color(s::c7)) == false);
    CHECK(board.is_under_attack(s::b7, c::white) == true);
    CHECK(board.is_under_attack(s::d6, board.what_color(s::d6)) == true);
    CHECK(board.is_under_attack(s::e5, board.what_color(s::e5)) == true);
    CHECK(board.is_under_attack(s::f4, board.what_color(s::f4)) == false);
    CHECK(board.is_under_attack(s::g3, board.what_color(s::g3)) == false);
    CHECK(board.is_under_attack(s::h2, board.what_color(s::h2)) == false);
    CHECK(board.is_under_attack(s::h1, board.what_color(s::h1)) == false);
    import_fen(&board, "7K/k6K/q6K/r5K1/b4K2/n3K3/p2K4/8 w - - 0 1");
    CHECK(board.is_under_attack(s::h8, board.what_color(s::h8)) == false);
    CHECK(board.is_under_attack(s::h7, board.what_color(s::h7)) == false);
    CHECK(board.is_under_attack(s::h6, board.what_color(s::h6)) == true);
    CHECK(board.is_under_attack(s::g5, board.what_color(s::g5)) == true);
    CHECK(board.is_under_attack(s::h5, c::white) == false);

}

TEST_CASE("horizontal right")
{
    import_fen(&board, "k7/k6K/k6Q/1k5R/2k4B/3k3N/4k2P/5k2 w - - 0 1");
    CHECK(export_fen(&board) == "k7/k6K/k6Q/1k5R/2k4B/3k3N/4k2P/5k2 w - - 0 1");
    CHECK(board.is_under_attack(s::a8, board.what_color(s::a8)) == false);
    CHECK(board.is_under_attack(s::a7, board.what_color(s::a7)) == false);
    CHECK(board.is_under_attack(s::a6, board.what_color(s::a6)) == true);
    CHECK(board.is_under_attack(s::b5, board.what_color(s::b5)) == true);
    CHECK(board.is_under_attack(s::c4, board.what_color(s::c4)) == false);
    CHECK(board.is_under_attack(s::d3, board.what_color(s::d3)) == false);
    CHECK(board.is_under_attack(s::e2, board.what_color(s::e2)) == false);
    CHECK(board.is_under_attack(s::f1, board.what_color(s::f1)) == false);
    CHECK(board.is_under_attack(s::a5, c::black) == false);
}

TEST_CASE("diagonal")
{
    import_fen(&board, "8/8/8/2k1k3/3p1P2/8/8/8 w - - 0 1");
    CHECK(board.is_under_attack(s::c5, board.what_color(s::c5)) == false);
    CHECK(board.is_under_attack(s::e5, board.what_color(s::e5)) == true);
    import_fen(&board, "8/8/3k4/2k5/3p1P2/8/8/8 w - - 0 1");
    CHECK(board.is_under_attack(s::d6, board.what_color(s::d6)) == false);
    import_fen(&board, "8/8/8/8/3P4/8/8/8 w - - 0 1");
    CHECK(board.is_under_attack(s::c5, c::black) == true);
    CHECK(board.is_under_attack(s::b5, c::black) == false);
    CHECK(board.is_white(s::d4) == true);
    CHECK(is_left_horizontal_boundary(s::e5) == false);
    CHECK(is_lower_vertical_boundary(s::e5) == false);
    CHECK(is_left_horizontal_boundary(s::d4 + 7) == false);
    CHECK(is_lower_vertical_boundary(s::d4 + 7) == false);
    CHECK(board.is_under_attack(s::e5, c::black) == true);
    CHECK(board.is_under_attack(s::f6, c::black) == false);
}
