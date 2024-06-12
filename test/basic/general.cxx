#include <catch2/catch_all.hpp>
#include <iostream>
#include "../../include/Board.h"

TEST_CASE("clear")
{
    Board board;
    SECTION("clear") {
        board.clear();
        CHECK(board.export_fen() == "8/8/8/8/8/8/8/8 w - - 0 0");
    }
}

TEST_CASE("export fen")
{
    Board board;
    std::string s;
    SECTION("starting position export") {
        CHECK(board.export_fen() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }
}

TEST_CASE("import fen")
{
    Board board;
    std::string s;
    SECTION("pos a") {
        s = "b5k1/2qn3p/p2p2pb/Np1Pp3/1P2P3/P1n2PBP/2rN4/R3QBK1 w - - 1 31";
        board.import_fen(s);
        CHECK(board.export_fen() == s);
    }SECTION("pos start") {
        s = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        board.import_fen(s);
        CHECK(board.export_fen() == s);
    }SECTION("pos en passant") {
        s = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1";
        board.import_fen(s);
        CHECK(board.export_fen() == s);
    }SECTION("castling") {
        s = "r1b1k2r/pp1nqpb1/2p1p1pp/3p4/2PP4/P1N1PN2/1P2BPPP/2RQ1RK1 b kq - 0 11";
        board.import_fen(s);
        CHECK(board.export_fen() == s);
    }
}

TEST_CASE("what_piece")
{
    Board board;
    CHECK(board.what_piece(0) == 'R');
    CHECK(board.what_piece(1) == 'N');
    CHECK(board.what_piece(2) == 'B');
    CHECK(board.what_piece(3) == 'K');
    CHECK(board.what_piece(4) == 'Q');
    CHECK(board.what_piece(5) == 'B');
    CHECK(board.what_piece(6) == 'N');
    CHECK(board.what_piece(7) == 'R');
    CHECK(board.what_piece(8) == 'P');
    CHECK(board.what_piece(9) == 'P');
    CHECK(board.what_piece(10) == 'P');
    CHECK(board.what_piece(11) == 'P');
    CHECK(board.what_piece(12) == 'P');
    CHECK(board.what_piece(13) == 'P');
    CHECK(board.what_piece(14) == 'P');
    CHECK(board.what_piece(15) == 'P');
    CHECK(board.what_piece(48) == 'p');
    CHECK(board.what_piece(49) == 'p');
    CHECK(board.what_piece(50) == 'p');
    CHECK(board.what_piece(51) == 'p');
    CHECK(board.what_piece(52) == 'p');
    CHECK(board.what_piece(53) == 'p');
    CHECK(board.what_piece(54) == 'p');
    CHECK(board.what_piece(55) == 'p');
    CHECK(board.what_piece(56) == 'r');
    CHECK(board.what_piece(57) == 'n');
    CHECK(board.what_piece(58) == 'b');
    CHECK(board.what_piece(59) == 'k');
    CHECK(board.what_piece(60) == 'q');
    CHECK(board.what_piece(61) == 'b');
    CHECK(board.what_piece(62) == 'n');
    CHECK(board.what_piece(63) == 'r');
}
