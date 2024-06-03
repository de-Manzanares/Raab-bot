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

}
