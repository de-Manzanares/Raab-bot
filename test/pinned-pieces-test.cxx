#include <catch2/catch_all.hpp>
#include <iostream>
#include "../src/Board.hpp"

void generate_and_sort(Board& board, const Square& sq, std::vector<Square>& v)
{
    v.clear();
    board.update_move_maps();
    Color c = board.what_color(sq);
    if (c == Color::white) { v = Maps::move_map_white[sq]; }
    if (c == Color::black) { v = Maps::move_map_black[sq]; }
    std::sort(v.begin(), v.end());
}

using s = Square;

Board board;
std::vector<Square> v;

TEST_CASE("pinned pieces rook")
{
    board.import_fen("8/8/8/1kppR2K/8/8/8/8 w - - 0 1");        // blocked
    CHECK(board.pinned_piece_rook(s::e5) == s::e5);
    board.import_fen("8/8/8/1k1rPP1K/8/8/8/8 w - - 0 1");
    CHECK(board.pinned_piece_rook(s::d5) == s::d5);
    board.import_fen("8/3K4/8/3R4/3p4/3P4/3k4/8 w - - 0 1");
    CHECK(board.pinned_piece_rook(s::d5) == s::d5);
    board.import_fen("8/3K4/3p4/3P4/3r4/8/3k4/8 w - - 0 1");
    CHECK(board.pinned_piece_rook(s::d4) == s::d4);

    board.import_fen("8/8/8/1k1pR2K/8/8/8/8 w - - 0 1");        // pinned
    CHECK(board.pinned_piece_rook(s::e5) == s::d5);
    board.import_fen("8/8/8/1k1rP2K/8/8/8/8 w - - 0 1");
    CHECK(board.pinned_piece_rook(s::d5) == s::e5);
    board.import_fen("8/3K4/8/3R4/3p4/8/3k4/8 w - - 0 1");
    CHECK(board.pinned_piece_rook(s::d5) == s::d4);
    board.import_fen("8/3K4/8/3P4/3r4/8/3k4/8 w - - 0 1");
    CHECK(board.pinned_piece_rook(s::d4) == s::d5);
}

TEST_CASE("pinned pieces bishop")
{
    board.import_fen("6k1/8/5p2/8/8/2B5/1K6/8 w - - 0 1");      // not pinned
    CHECK(board.pinned_piece_bishop(s::c3) == s::c3);
    board.import_fen("8/6k1/5b2/8/8/2P5/K7/8 w - - 0 1");
    CHECK(board.pinned_piece_bishop(s::f6) == s::f6);
    board.import_fen("8/1K6/2B5/8/8/5p1k/8/8 w - - 0 1");
    CHECK(board.pinned_piece_bishop(s::c6) == s::c6);
    board.import_fen("8/K7/2P5/8/8/5b2/6k1/8 w - - 0 1");
    CHECK(board.pinned_piece_bishop(s::f3) == s::f3);

    board.import_fen("8/6k1/5p2/8/8/2B5/1K6/8 w - - 0 1");      // pinned
    CHECK(board.pinned_piece_bishop(s::c3) == s::f6);
    board.import_fen("8/6k1/5b2/8/8/2P5/1K6/8 w - - 0 1");
    CHECK(board.pinned_piece_bishop(s::f6) == s::c3);
    board.import_fen("8/1K6/2B5/8/8/5p2/6k1/8 w - - 0 1");
    CHECK(board.pinned_piece_bishop(s::c6) == s::f3);
    board.import_fen("8/1K6/2P5/8/8/5b2/6k1/8 w - - 0 1");
    CHECK(board.pinned_piece_bishop(s::f3) == s::c6);

}

TEST_CASE("lanes rook")
{
    board.import_fen("8/8/8/k2r1R1K/8/8/8/8 w - - 0 1");        // pinned
    generate_and_sort(board, s::d5, v);
    CHECK(v == std::vector<Square>{s::f5, s::e5, s::c5, s::b5});
    board.import_fen("8/8/8/kr4QK/8/8/8/8 w - - 0 1");
    generate_and_sort(board, s::g5, v);
    CHECK(v == std::vector<Square>{s::f5, s::e5, s::d5, s::c5, s::b5});
    board.import_fen("8/3K4/8/3R4/3b4/8/3k4/8 w - - 0 1");
    generate_and_sort(board, s::d4, v);
    CHECK(v == std::vector<Square>{});
    board.import_fen("8/3K4/8/3N4/3r4/8/3k4/8 w - - 0 1");
    generate_and_sort(board, s::d5, v);
    CHECK(v == std::vector<Square>{});
}

TEST_CASE("lanes bishop")
{
    board.import_fen("8/6k1/5b2/8/8/2B5/1K6/8 w - - 0 1");      // pinned
    generate_and_sort(board, s::f6, v);
    CHECK(v == std::vector<Square>{s::c3, s::d4, s::e5});
    board.import_fen("8/6k1/5b2/8/8/2Q5/1K6/8 w - - 0 1");
    generate_and_sort(board, s::c3, v);
    CHECK(v == std::vector<Square>{s::d4, s::e5, s::f6});
    board.import_fen("8/1K6/2B5/8/8/5n2/6k1/8 w - - 0 1");
    generate_and_sort(board, s::f3, v);
    CHECK(v == std::vector<Square>{});
    board.import_fen("8/1K6/2P5/8/8/5q2/6k1/8 w - - 0 1");
    generate_and_sort(board, s::c6, v);
    CHECK(v == std::vector<Square>{});
}