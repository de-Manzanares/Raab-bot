/**
 * @file test0.cxx
 * movegen performance test
 * @see https://www.chessprogramming.org/Perft_Results
 */

#include "defs.h"

#include <catch2/catch_all.hpp>

// expected movegen performance test results
constexpr U64 perft_res[7][7] = {
    {0,  0,    0,     0,       0,         0,          0},
    {0, 20,  400,  8902,  197281,   4865609,  119060324},
    {0, 48, 2039, 97862, 4085603, 193690690, 8031647685},
    {0, 14,  191,  2812,   43238,    674624,   11030083},
    {0,  6,  264,  9467,  422333,  15833292,  706045033},
    {0, 44, 1486, 62379, 2103487,  89941194,          0},
    {0, 46, 2079, 89890, 3894594, 164075551, 6923051137}
};

struct PerfTCounts {
  U64 captures{};
  U64 ep{};
  U64 castles{};
  U64 promotions{};
  U64 checks{};
  U64 checkmates{};
};

TEST_CASE("test_case")
{
  SECTION("section") { CHECK(true == true); }
}
