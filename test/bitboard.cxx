#include <catch2/catch_all.hpp>
#include <iostream>
#include "../../../../ChessEngine/src/bitboard.h"

TEST_CASE("fen")
{
    Board board;
    CHECK(export_fen(&board) == "r");
}

