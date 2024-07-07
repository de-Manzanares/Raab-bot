#include <Eval.h>
#include <Search.h>
#include <UCI.h>
#include <catch2/catch_all.hpp>
#include <memory>

double test_s(const std::string &fen) {
  const auto n = make_shared<Node>(fen);
  n->board()->update_move_maps();
  return Eval::stacked_pawns(n.get()) / Eval::STACKED_PAWN_PENALTY;
}

double test_p(const std::string &fen) {
  const auto n = std::make_shared<Node>(fen);
  n->board()->update_move_maps();
  return Eval::passed_pawns(n.get()) / Eval::PASSED_PAWN_BONUS;
}

TEST_CASE("stacked pawns") {
  CHECK(test_s("8/3k1p2/8/3P4/3P4/8/2K5/8 w - - 0 1") == Catch::Approx(-1));
  CHECK(test_s("8/3k1p2/8/3P1P2/3P1P2/8/2K5/8 w - - 0 1") == Catch::Approx(-2));
  CHECK(test_s("8/3k1p2/5P2/3P1P2/3P1P2/8/2K5/8 w - - 0 1") ==
        Catch::Approx(-3));
  CHECK(test_s("8/3k1p2/1p3p2/7p/1P1P4/P4PP1/2K5/8 w - - 0 1") ==
        Catch::Approx(1));
  CHECK(test_s("8/3k1p2/1p3p2/3P3p/1P1P4/P4PP1/2K5/8 w - - 0 1") ==
        Catch::Approx(0));
  CHECK(test_s("8/3k1p2/1p3p2/1p1P1p1p/1P1P3p/P4PP1/2K5/8 w - - 0 1") ==
        Catch::Approx(3));
}

TEST_CASE("passed pawns") {
  CHECK(test_p("5k2/8/8/8/8/8/8/2K5 w - - 0 1") == Catch::Approx(0));
  CHECK(test_p("5k2/8/8/8/2P5/8/8/2K5 w - - 0 1") == Catch::Approx(1));
  CHECK(test_p("5k2/8/8/8/P7/8/8/2K5 w - - 0 1") == Catch::Approx(1));
  CHECK(test_p("5k2/8/8/p7/P7/8/8/2K5 w - - 0 1") == Catch::Approx(0));
  CHECK(test_p("5k2/8/8/pP6/P7/8/8/2K5 w - - 0 1") == Catch::Approx(1));
  CHECK(test_p("5k2/8/1P6/pP6/P7/8/8/2K5 w - - 0 1") == Catch::Approx(2));
  CHECK(test_p("5k2/8/1Pp5/pP6/P7/8/8/2K5 w - - 0 1") == Catch::Approx(1));
  CHECK(test_p("5k2/8/1Pp5/pPp5/P7/8/8/2K5 w - - 0 1") == Catch::Approx(0));
  CHECK(test_p("5k2/8/7p/1P2Pp1P/2Pp1PP1/8/8/2K5 w - - 0 1") ==
        Catch::Approx(2));
}
