#include <catch2/catch_all.hpp>
#include <iostream>
#include "../../include/Board.h"

using s = Square;

Board board_sgt;

// little play test
// 1.d4 Nf6
// 2.c4 c6
// 3.Nc3 d5
// 4.Nf3 dxc4
// 5.a4 Bf5
// 6.Nh4 Bg4
// 7.h3 Bd7
// 8.e4 e6
// 9.Bxc4 Nxe4
// 10.Nxe4 *

std::vector<int> number_of_legal_moves = {
        0,
        20, 20,
        28, 22,
        30, 23,
        33, 31,
        34, 33,
        34, 39,
        32, 36,
        33, 27,
        40, 29,
        48, 29
};
int i = 1;
TEST_CASE("sample game")
{
    board_sgt.import_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    CHECK(board_sgt.nodes_at_depth_1(Color::white) == number_of_legal_moves[i++]);
    board_sgt.move(s::d2, s::d4, 0);
    CHECK(board_sgt.export_fen() == "rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq d3 0 1");

    CHECK(board_sgt.nodes_at_depth_1(Color::black) == number_of_legal_moves[i++]);
    board_sgt.move(s::g8, s::f6, 0);
    CHECK(board_sgt.export_fen() == "rnbqkb1r/pppppppp/5n2/8/3P4/8/PPP1PPPP/RNBQKBNR w KQkq - 1 2");

    CHECK(board_sgt.nodes_at_depth_1(Color::white) == number_of_legal_moves[i++]);
    board_sgt.move(s::c2, s::c4, 0);
    CHECK(board_sgt.export_fen() == "rnbqkb1r/pppppppp/5n2/8/2PP4/8/PP2PPPP/RNBQKBNR b KQkq c3 0 2");

    CHECK(board_sgt.nodes_at_depth_1(Color::black) == number_of_legal_moves[i++]);
    board_sgt.move(s::c7, s::c6, 0);
    CHECK(board_sgt.export_fen() == "rnbqkb1r/pp1ppppp/2p2n2/8/2PP4/8/PP2PPPP/RNBQKBNR w KQkq - 0 3");

    CHECK(board_sgt.nodes_at_depth_1(Color::white) == number_of_legal_moves[i++]);
    board_sgt.move(s::b1, s::c3, 0);
    CHECK(board_sgt.export_fen() == "rnbqkb1r/pp1ppppp/2p2n2/8/2PP4/2N5/PP2PPPP/R1BQKBNR b KQkq - 1 3");

    CHECK(board_sgt.nodes_at_depth_1(Color::black) == number_of_legal_moves[i++]);
    board_sgt.move(s::d7, s::d5, 0);
    CHECK(board_sgt.export_fen() == "rnbqkb1r/pp2pppp/2p2n2/3p4/2PP4/2N5/PP2PPPP/R1BQKBNR w KQkq d6 0 4");

    CHECK(board_sgt.nodes_at_depth_1(Color::white) == number_of_legal_moves[i++]);
    board_sgt.move(s::g1, s::f3, 0);
    CHECK(board_sgt.export_fen() == "rnbqkb1r/pp2pppp/2p2n2/3p4/2PP4/2N2N2/PP2PPPP/R1BQKB1R b KQkq - 1 4");

    CHECK(board_sgt.nodes_at_depth_1(Color::black) == number_of_legal_moves[i++]);
    board_sgt.move(s::d5, s::c4, 0);
    CHECK(board_sgt.export_fen() == "rnbqkb1r/pp2pppp/2p2n2/8/2pP4/2N2N2/PP2PPPP/R1BQKB1R w KQkq - 0 5");

    CHECK(board_sgt.nodes_at_depth_1(Color::white) == number_of_legal_moves[i++]);
    board_sgt.move(s::a2, s::a4, 0);
    CHECK(board_sgt.export_fen() == "rnbqkb1r/pp2pppp/2p2n2/8/P1pP4/2N2N2/1P2PPPP/R1BQKB1R b KQkq a3 0 5");

    CHECK(board_sgt.nodes_at_depth_1(Color::black) == number_of_legal_moves[i++]);
    board_sgt.move(s::c8, s::f5, 0);
    CHECK(board_sgt.export_fen() == "rn1qkb1r/pp2pppp/2p2n2/5b2/P1pP4/2N2N2/1P2PPPP/R1BQKB1R w KQkq - 1 6");

    CHECK(board_sgt.nodes_at_depth_1(Color::white) == number_of_legal_moves[i++]);
    board_sgt.move(s::f3, s::h4, 0);
    CHECK(board_sgt.export_fen() == "rn1qkb1r/pp2pppp/2p2n2/5b2/P1pP3N/2N5/1P2PPPP/R1BQKB1R b KQkq - 2 6");

    CHECK(board_sgt.nodes_at_depth_1(Color::black) == number_of_legal_moves[i++]);
    board_sgt.move(s::f5, s::g4, 0);
    CHECK(board_sgt.export_fen() == "rn1qkb1r/pp2pppp/2p2n2/8/P1pP2bN/2N5/1P2PPPP/R1BQKB1R w KQkq - 3 7");

    CHECK(board_sgt.nodes_at_depth_1(Color::white) == number_of_legal_moves[i++]);
    board_sgt.move(s::h2, s::h3, 0);
    CHECK(board_sgt.export_fen() == "rn1qkb1r/pp2pppp/2p2n2/8/P1pP2bN/2N4P/1P2PPP1/R1BQKB1R b KQkq - 0 7");

    CHECK(board_sgt.nodes_at_depth_1(Color::black) == number_of_legal_moves[i++]);
    board_sgt.move(s::g4, s::d7, 0);
    CHECK(board_sgt.export_fen() == "rn1qkb1r/pp1bpppp/2p2n2/8/P1pP3N/2N4P/1P2PPP1/R1BQKB1R w KQkq - 1 8");

    CHECK(board_sgt.nodes_at_depth_1(Color::white) == number_of_legal_moves[i++]);
    board_sgt.move(s::e2, s::e4, 0);
    CHECK(board_sgt.export_fen() == "rn1qkb1r/pp1bpppp/2p2n2/8/P1pPP2N/2N4P/1P3PP1/R1BQKB1R b KQkq e3 0 8");

    CHECK(board_sgt.nodes_at_depth_1(Color::black) == number_of_legal_moves[i++]);
    board_sgt.move(s::e7, s::e6, 0);
    CHECK(board_sgt.export_fen() == "rn1qkb1r/pp1b1ppp/2p1pn2/8/P1pPP2N/2N4P/1P3PP1/R1BQKB1R w KQkq - 0 9");

    CHECK(board_sgt.nodes_at_depth_1(Color::white) == number_of_legal_moves[i++]);
    board_sgt.move(s::f1, s::c4, 0);
    CHECK(board_sgt.export_fen() == "rn1qkb1r/pp1b1ppp/2p1pn2/8/P1BPP2N/2N4P/1P3PP1/R1BQK2R b KQkq - 0 9");

    CHECK(board_sgt.nodes_at_depth_1(Color::black) == number_of_legal_moves[i++]);
    board_sgt.move(s::f6, s::e4, 0);
    CHECK(board_sgt.export_fen() == "rn1qkb1r/pp1b1ppp/2p1p3/8/P1BPn2N/2N4P/1P3PP1/R1BQK2R w KQkq - 0 10");

    CHECK(board_sgt.nodes_at_depth_1(Color::white) == number_of_legal_moves[i++]);
    board_sgt.move(s::c3, s::e4, 0);
    CHECK(board_sgt.export_fen() == "rn1qkb1r/pp1b1ppp/2p1p3/8/P1BPN2N/7P/1P3PP1/R1BQK2R b KQkq - 0 10");
}

TEST_CASE("moving rooks updating castling rights")
{
    board_sgt.import_fen("r3k2r/pp1bbppp/nqp1p3/8/P1BPN2N/3QBP1P/1P4P1/R3K2R b KQkq - 4 13");
    board_sgt.move(s::h8, s::g8, 0);
    CHECK(board_sgt.export_fen() == "r3k1r1/pp1bbppp/nqp1p3/8/P1BPN2N/3QBP1P/1P4P1/R3K2R w KQq - 5 14");

    board_sgt.import_fen("r3k2r/pp1bbppp/nqp1p3/8/P1BPN2N/3QBP1P/1P4P1/R3K2R b KQkq - 4 13");
    board_sgt.move(s::a8, s::b8, 0);
    CHECK(board_sgt.export_fen() == "1r2k2r/pp1bbppp/nqp1p3/8/P1BPN2N/3QBP1P/1P4P1/R3K2R w KQk - 5 14");

    board_sgt.import_fen("r3k2r/pp1bb1pp/nqp1pp2/8/P1BPN2N/3QBP1P/1P4P1/R3K2R w KQkq - 0 14");
    board_sgt.move(s::a1, s::b1, 0);
    CHECK(board_sgt.export_fen() == "r3k2r/pp1bb1pp/nqp1pp2/8/P1BPN2N/3QBP1P/1P4P1/1R2K2R b Kkq - 1 14");

    board_sgt.import_fen("r3k2r/pp1bb1pp/nqp1pp2/8/P1BPN2N/3QBP1P/1P4P1/R3K2R w KQkq - 0 14");
    board_sgt.move(s::h1, s::g1, 0);
    CHECK(board_sgt.export_fen() == "r3k2r/pp1bb1pp/nqp1pp2/8/P1BPN2N/3QBP1P/1P4P1/R3K1R1 b Qkq - 1 14");
}

TEST_CASE("moving kings update castling rights")
{
    board_sgt.import_fen("r3k2r/pp1bb1pp/nqp1pp2/8/P1BPN2N/3QBP1P/1P4P1/R3K2R w KQkq - 0 14");
    board_sgt.move(s::e1, s::e2, 0);
    CHECK(board_sgt.export_fen() == "r3k2r/pp1bb1pp/nqp1pp2/8/P1BPN2N/3QBP1P/1P2K1P1/R6R b kq - 1 14");

    board_sgt.import_fen("r3k2r/pp1bbppp/nqp1p3/8/P1BPN2N/3QBP1P/1P4P1/R3K2R b KQkq - 4 13");
    board_sgt.move(s::e8, s::d8, 0);
    CHECK(board_sgt.export_fen() == "r2k3r/pp1bbppp/nqp1p3/8/P1BPN2N/3QBP1P/1P4P1/R3K2R w KQ - 5 14");
}

TEST_CASE("castle")
{
    board_sgt.import_fen("r3k2r/pp1bbppp/nqp1p3/8/P1BPN2N/3QBP1P/1P4P1/R3K2R b KQkq - 4 13");
    board_sgt.move(s::e8, s::g8, 0);
    CHECK(board_sgt.export_fen() == "r4rk1/pp1bbppp/nqp1p3/8/P1BPN2N/3QBP1P/1P4P1/R3K2R w KQ - 5 14");
    board_sgt.import_fen("r3k2r/pp1bbppp/nqp1p3/8/P1BPN2N/3QBP1P/1P4P1/R3K2R b KQkq - 4 13");
    board_sgt.move(s::e8, s::c8, 0);
    CHECK(board_sgt.export_fen() == "2kr3r/pp1bbppp/nqp1p3/8/P1BPN2N/3QBP1P/1P4P1/R3K2R w KQ - 5 14");

    board_sgt.move(s::e1, s::g1, 0);
    CHECK(board_sgt.export_fen() == "2kr3r/pp1bbppp/nqp1p3/8/P1BPN2N/3QBP1P/1P4P1/R4RK1 b - - 6 14");
    board_sgt.import_fen("2kr3r/pp1bbppp/nqp1p3/8/P1BPN2N/3QBP1P/1P4P1/R3K2R w KQ - 5 14");
    board_sgt.move(s::e1, s::c1, 0);
    CHECK(board_sgt.export_fen() == "2kr3r/pp1bbppp/nqp1p3/8/P1BPN2N/3QBP1P/1P4P1/2KR3R b - - 6 14");

}

TEST_CASE("pawn promotion")
{
    board_sgt.import_fen("8/1KP5/8/8/8/8/5pk1/8 w - - 0 1");
    board_sgt.move(s::c7, s::c8, 'q');
    CHECK(board_sgt.export_fen() == "2Q5/1K6/8/8/8/8/5pk1/8 b - - 0 1");
    board_sgt.import_fen("8/1KP5/8/8/8/8/5pk1/8 w - - 0 1");
    board_sgt.move(s::c7, s::c8, 'r');
    CHECK(board_sgt.export_fen() == "2R5/1K6/8/8/8/8/5pk1/8 b - - 0 1");
    board_sgt.import_fen("8/1KP5/8/8/8/8/5pk1/8 w - - 0 1");
    board_sgt.move(s::c7, s::c8, 'b');
    CHECK(board_sgt.export_fen() == "2B5/1K6/8/8/8/8/5pk1/8 b - - 0 1");
    board_sgt.import_fen("8/1KP5/8/8/8/8/5pk1/8 w - - 0 1");
    board_sgt.move(s::c7, s::c8, 'n');
    CHECK(board_sgt.export_fen() == "2N5/1K6/8/8/8/8/5pk1/8 b - - 0 1");

    board_sgt.import_fen("8/1KP5/8/8/8/8/5pk1/8 b - - 0 2");
    board_sgt.move(s::f2, s::f1, 'q');
    CHECK(board_sgt.export_fen() == "8/1KP5/8/8/8/8/6k1/5q2 w - - 0 3");
    board_sgt.import_fen("8/1KP5/8/8/8/8/5pk1/8 b - - 0 2");
    board_sgt.move(s::f2, s::f1, 'r');
    CHECK(board_sgt.export_fen() == "8/1KP5/8/8/8/8/6k1/5r2 w - - 0 3");
    board_sgt.import_fen("8/1KP5/8/8/8/8/5pk1/8 b - - 0 2");
    board_sgt.move(s::f2, s::f1, 'b');
    CHECK(board_sgt.export_fen() == "8/1KP5/8/8/8/8/6k1/5b2 w - - 0 3");
    board_sgt.import_fen("8/1KP5/8/8/8/8/5pk1/8 b - - 0 2");
    board_sgt.move(s::f2, s::f1, 'n');
    CHECK(board_sgt.export_fen() == "8/1KP5/8/8/8/8/6k1/5n2 w - - 0 3");
}
