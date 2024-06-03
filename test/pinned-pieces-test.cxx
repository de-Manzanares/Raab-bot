#include <catch2/catch_all.hpp>
#include <iostream>
#include "../src/Board.hpp"

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
