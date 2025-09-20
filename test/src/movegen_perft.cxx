#include <catch2/catch_all.hpp>

// https://www.chessprogramming.org/Perft_Results

std::size_t perft_results[7][7] = {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 20, 400, 8902, 197281, 4865609, 119060324},
    {0, 48, 2039, 97862, 4085603, 193690690, 8031647685},
    {0, 14, 191, 2812, 43238, 674624, 11030083},
    {0, 6, 264, 9467, 422333, 15833292, 706045033},
    {0, 44, 1486, 62379, 2103487, 89941194, 0},
    {0, 46, 2079, 89890, 3894594, 164075551, 6923051137}};

import Board0x88;

TEST_CASE("perft initial") {
  Board b0;
  SECTION("depth 4") { CHECK(movegen_perft(b0, 4) == 197281); }
  SECTION("depth 5") { CHECK(movegen_perft(b0, 5) == 4865609); }
}

TEST_CASE("perft position 2") {
  Board b0("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
  SECTION("depth 2") { CHECK(movegen_perft(b0, 2) == perft_results[2][2]); }
  SECTION("depth 3") { CHECK(movegen_perft(b0, 3) == perft_results[2][3]); }
}

TEST_CASE("perft position 3") {
  Board b0("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 ");
  SECTION("depth 4") { CHECK(movegen_perft(b0, 4) == perft_results[3][4]); }
  SECTION("depth 5") { CHECK(movegen_perft(b0, 5) == perft_results[3][5]); }
}

TEST_CASE("perft position 4") {
  Board b0("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
  SECTION("depth 2") { CHECK(movegen_perft(b0, 2) == 264); }
  SECTION("depth 3") { CHECK(movegen_perft(b0, 3) == 9467); }
}

TEST_CASE("perft position 5") {
  Board b0("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ");
  SECTION("depth 1") { CHECK(movegen_perft(b0, 1) == perft_results[5][1]); }
  SECTION("depth 2") { CHECK(movegen_perft(b0, 2) == perft_results[5][2]); }
}

TEST_CASE("perft position 6") {
  Board b0("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w "
           "- - 0 10 ");
  SECTION("depth 4") { CHECK(movegen_perft(b0, 4) == perft_results[6][4]); }
}
