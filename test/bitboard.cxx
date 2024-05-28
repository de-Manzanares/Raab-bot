#include <catch2/catch_all.hpp>
#include <iostream>
#include "../src/bitboard.h"

TEST_CASE("fen")
{
    Board board;
    SECTION("starting position export")
    CHECK(export_fen(&board) == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

