#include <catch2/catch_all.hpp>

import Board0x88;

TEST_CASE("perft initial") {
  SECTION("depth 1") {
    Board b0;
    CHECK(movegen(b0).move_count == 20);
  }
}

TEST_CASE("perft position 2") {
  SECTION("depth 1") {
    Board b0(
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
    CHECK(movegen(b0).move_count == 48);
  }
}

TEST_CASE("perft position 3") {
  SECTION("depth 1") {
    Board b0("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 ");
    CHECK(movegen(b0).move_count == 14);
  }
}

TEST_CASE("perft position 4") {
  SECTION("depth 1") {
    Board b0(
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    CHECK(search(b0, 0) == 6);
  }
}

TEST_CASE("perft position 5") {
  SECTION("depth 1") {
    Board b0("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ");
    CHECK(movegen(b0).move_count == 44);
  }
}

TEST_CASE("perft position 6") {
  SECTION("depth 1") {
    Board b0("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w "
             "- - 0 10 ");
    CHECK(movegen(b0).move_count == 1);
  }
}
