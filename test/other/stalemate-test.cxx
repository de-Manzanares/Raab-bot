#include "Search.h"
#include "UCI.h"

#include <catch2/catch_all.hpp>

std::string test(const std::string &fen) {
  const auto n = make_shared<Node>(fen);
  const bool maxing = uciloop::is_maxing(n);
  n->board()->update_move_maps();
  n->spawn_depth_first(3);
  const std::shared_ptr<Node> opt =
      Search::min_max(n, 3, -100'000, 100'000, maxing);
  const std::shared_ptr<Node> best = n->next_step(opt);
  return uciloop::long_algebraic_notation(best);
}

std::string s;
std::string stalemate;

TEST_CASE("white to move") {
  SECTION("https://lichess.org/KptGwD50/white#98") {
    s = test("4Nk2/2R4R/1P3P1P/6P1/8/3K4/8/8 w - - 1 50");
    stalemate = "f6f7";
    CHECK(!uciloop::simon_says(&s, stalemate)); // don't choose stalemate
  }
  SECTION("https://lichess.org/cR3Gu4XS/white#116") {
    s = test("5k2/2R5/3P1P1P/p7/Pp2p2P/1P2P3/5K2/8 w - - 3 59");
    stalemate = "f6f7";
    CHECK(!uciloop::simon_says(&s, stalemate)); // don't choose stalemate
  }
}
TEST_CASE("black to move") {
  SECTION("https://lichess.org/EQEInzg9/black#59") {
    s = test("r4b1r/pp3k2/P7/2p3pp/4n3/3p3K/4q2P/n7 b - - 0 30");
    stalemate = "b7a6";
    CHECK(!uciloop::simon_says(&s, stalemate)); // don't choose stalemate
  }
  SECTION("https://lichess.org/rT2Ny1Vb/black#105") {
    s = test("8/3k4/1p5p/3b4/2r2p2/1p6/3K4/7q b - - 1 53");
    stalemate = "h1f1";
    CHECK(!uciloop::simon_says(&s, stalemate)); // don't choose stalemate
  }
}
