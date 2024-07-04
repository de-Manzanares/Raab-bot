#include <Eval.h>
#include <Search.h>
#include <UCI.h>
#include <catch2/catch_all.hpp>

double test_spt(const std::string &fen) {
  const auto n = make_shared<Node>(fen);
  n->board()->update_move_maps();
  return Eval::stacked_pawns(n.get());
}

TEST_CASE("stacked pawns") {
  CHECK(test_spt("8/3k1p2/8/3P4/3P4/8/2K5/8 w - - 0 1") == -0.25);
  CHECK(test_spt("8/3k1p2/8/3P1P2/3P1P2/8/2K5/8 w - - 0 1") == -0.5);
  CHECK(test_spt("8/3k1p2/5P2/3P1P2/3P1P2/8/2K5/8 w - - 0 1") == -0.75);
  CHECK(test_spt("8/3k1p2/1p3p2/7p/1P1P4/P4PP1/2K5/8 w - - 0 1") == .25);
  CHECK(test_spt("8/3k1p2/1p3p2/3P3p/1P1P4/P4PP1/2K5/8 w - - 0 1") == 0);
  CHECK(test_spt("8/3k1p2/1p3p2/1p1P1p1p/1P1P3p/P4PP1/2K5/8 w - - 0 1") == .75);
}
