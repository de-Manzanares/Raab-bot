/**
 *@file search.cxx
 * not so much a "test" as me just mucking around in the debugger
 */

#include <catch2/catch_all.hpp>
#include <iostream>

import Board0x88;

constexpr int alpha = std::numeric_limits<int>::min() / 2;
constexpr int beta = std::numeric_limits<int>::max() / 2;

TEST_CASE("trial") {
  Board b("8/1bprrpkp/1p3RpN/p1p3P1/P3P2P/1PqP1Q2/2P5/5R1K w - - 7 35");
  std::cout << alpha_beta_root(b, alpha, beta, 3) << '\n';
}

TEST_CASE("startpos") {
  Board b;
  std::cout << alpha_beta_root(b, alpha, beta, 3) << '\n';
}

TEST_CASE("questionable capture") {
  Board b("rnbqkbnr/1p1ppppp/2p5/8/2B1P3/N5Q1/PPPP1PPP/R1B1K1NR b KQkq - 0 6");
  std::cout << alpha_beta_root(b, alpha, beta, 5) << '\n';
}

TEST_CASE("mate in 1") {
  Board b("rnb1k1nr/pppp1ppp/8/2b1p3/7q/N7/PPPPPPPP/1RBQKBNR b Kkq - 5 4");
  SECTION("alpha beta") {
    CHECK(Move{.from_sq = c5, .to_sq = f2, .flag = capture} ==
          alpha_beta_root(b, alpha, beta, 5));
  }
  SECTION("quiesce") { CHECK(1ULL << 30 == quiesce(b, alpha, beta, 0)); }
}

TEST_CASE("avoid mate in 1") {
  Board b("rnb1k1nr/pppp1ppp/8/2b1p3/7q/N7/PPPPPPPP/R1BQKBNR w Kkq - 4 4");
  std::cout << alpha_beta_root(b, alpha, beta, 5) << '\n';
}

TEST_CASE("restrict the king") {
  Board b("8/p4kp1/6p1/2R5/8/PP3P2/5KPP/8 w - - 7 39");
  std::cout << alpha_beta_root(b, alpha, beta, 5) << '\n';
}

TEST_CASE("mate in 1 as white") {
  Board b("2r2k2/6RR/4p2B/1r2P2P/8/3P3P/4KP2/8 w - - 6 39");
  std::cout << alpha_beta_root(b, alpha, beta, 5) << '\n';
}

// clang-format off
// haven't fixed it yet. gave a2a1r, but thought it was a queen later on
// position startpos moves e2e4 e7e5 d2d3 g8f6 b1d2 b7b5 d1f3 d7d6 d3d4 a7a6 d4e5 d6e5 f1e2 b8c6 f3c3 c6d4 c3d3 c8e6 c2c3 d4e2 d3e2 e6g4 f2f3 g4e6 d2f1 e6c4 e2e3 c4f1 e1f1 d8d1 f1f2 f8e7 h2h4 a6a5 h4h5 h7h6 f2g3 a8d8 e3f2 d8d3 h1h4 f6d7 h4g4 e7c5 f2e2 d7f6 e2d1 d3d1 g4g7 f6h5 g3g4 h5g7 g1e2 c5e3 e2g3 h6h5 g3h5 h8h5 f3f4 d1c1 a1c1 e3c1 f4e5 c1b2 e5e6 f7e6 a2a4 b5a4 c3c4 h5c5 g4g3 g7h5 g3g4 h5f6 g4f4 c5c4 f4f3 b2c3 f3f4 c3d4 e4e5 f6d7 f4f3 c4c2 f3e4 d4g1 g2g3 c2c3 e4f4 g1h2 f4g4 c3g3 g4f4 a4a3 f4e4 a3a2 e4f4
// go movetime 45000
// bestmove a2a1r
// position startpos moves e2e4 e7e5 d2d3 g8f6 b1d2 b7b5 d1f3 d7d6 d3d4 a7a6 d4e5 d6e5 f1e2 b8c6 f3c3 c6d4 c3d3 c8e6 c2c3 d4e2 d3e2 e6g4 f2f3 g4e6 d2f1 e6c4 e2e3 c4f1 e1f1 d8d1 f1f2 f8e7 h2h4 a6a5 h4h5 h7h6 f2g3 a8d8 e3f2 d8d3 h1h4 f6d7 h4g4 e7c5 f2e2 d7f6 e2d1 d3d1 g4g7 f6h5 g3g4 h5g7 g1e2 c5e3 e2g3 h6h5 g3h5 h8h5 f3f4 d1c1 a1c1 e3c1 f4e5 c1b2 e5e6 f7e6 a2a4 b5a4 c3c4 h5c5 g4g3 g7h5 g3g4 h5f6 g4f4 c5c4 f4f3 b2c3 f3f4 c3d4 e4e5 f6d7 f4f3 c4c2 f3e4 d4g1 g2g3 c2c3 e4f4 g1h2 f4g4 c3g3 g4f4 a4a3 f4e4 a3a2 e4f4 a2a1r f4e4
// go movetime 45000
// bestmove d7e5
// position startpos moves e2e4 e7e5 d2d3 g8f6 b1d2 b7b5 d1f3 d7d6 d3d4 a7a6 d4e5 d6e5 f1e2 b8c6 f3c3 c6d4 c3d3 c8e6 c2c3 d4e2 d3e2 e6g4 f2f3 g4e6 d2f1 e6c4 e2e3 c4f1 e1f1 d8d1 f1f2 f8e7 h2h4 a6a5 h4h5 h7h6 f2g3 a8d8 e3f2 d8d3 h1h4 f6d7 h4g4 e7c5 f2e2 d7f6 e2d1 d3d1 g4g7 f6h5 g3g4 h5g7 g1e2 c5e3 e2g3 h6h5 g3h5 h8h5 f3f4 d1c1 a1c1 e3c1 f4e5 c1b2 e5e6 f7e6 a2a4 b5a4 c3c4 h5c5 g4g3 g7h5 g3g4 h5f6 g4f4 c5c4 f4f3 b2c3 f3f4 c3d4 e4e5 f6d7 f4f3 c4c2 f3e4 d4g1 g2g3 c2c3 e4f4 g1h2 f4g4 c3g3 g4f4 a4a3 f4e4 a3a2 e4f4 a2a1r f4e4 d7e5 e4e5
// go movetime 45000
// bestmove g3h3
// position startpos moves e2e4 e7e5 d2d3 g8f6 b1d2 b7b5 d1f3 d7d6 d3d4 a7a6 d4e5 d6e5 f1e2 b8c6 f3c3 c6d4 c3d3 c8e6 c2c3 d4e2 d3e2 e6g4 f2f3 g4e6 d2f1 e6c4 e2e3 c4f1 e1f1 d8d1 f1f2 f8e7 h2h4 a6a5 h4h5 h7h6 f2g3 a8d8 e3f2 d8d3 h1h4 f6d7 h4g4 e7c5 f2e2 d7f6 e2d1 d3d1 g4g7 f6h5 g3g4 h5g7 g1e2 c5e3 e2g3 h6h5 g3h5 h8h5 f3f4 d1c1 a1c1 e3c1 f4e5 c1b2 e5e6 f7e6 a2a4 b5a4 c3c4 h5c5 g4g3 g7h5 g3g4 h5f6 g4f4 c5c4 f4f3 b2c3 f3f4 c3d4 e4e5 f6d7 f4f3 c4c2 f3e4 d4g1 g2g3 c2c3 e4f4 g1h2 f4g4 c3g3 g4f4 a4a3 f4e4 a3a2 e4f4 a2a1r f4e4 d7e5 e4e5 g3h3 e5e6
// go movetime 45000
// bestmove a1e5
// clang-format on
TEST_CASE("underpromotion") {
  Board b("4k3/2pn4/4p3/p3P3/5K2/6r1/p6b/8 b - - 1 49");
  std::cout << alpha_beta_root(b, alpha, beta, 4) << '\n';
}