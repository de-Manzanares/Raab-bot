#include "UCI.h"
#include <catch2/catch_all.hpp>
#include <iostream>

TEST_CASE("https://lichess.org/EKazIsEA") {
  // Save original cout and cin
  std::streambuf *original_cout = std::cout.rdbuf();
  std::streambuf *original_cin = std::cin.rdbuf();

  // Set up input and output streams
  std::istringstream test_input(
      "position startpos moves b1c3 c7c5 d2d4 c5d4 d1d4 b8c6 d4d3 c6b4 d3e4 "
      "d8b6 a2a3 d7d5 e4b4 b6c6 g1f3 e7e5 f3e5 f8b4 e5c6 b4c3 b2c3 b7c6 c1f4 "
      "c8a6 e2e4 a6f1 e1f1 d5e4 a1b1 a8d8 f4e3 a7a6 e3b6 d8b8 b1b3 c6c5 b6c5 "
      "b8c8 c5d6 e8d7 d6f4 d7e8 b3b8 c8b8 f4b8 e8d7 b8f4 g8e7 f4e5 h8c8 e5g7 "
      "e7f5 g7e5 c8c5 e5d4 f5d4 c3d4 c5c2 g2g4 c2c1 f1g2 c1c2 h1e1 d7c7 g4g5 "
      "e4e3 e1e3 c7d7 a3a4 a6a5 h2h3 c2d2 e3f3 f7f6 g5f6 d2d4 f6f7 d4f4 f3f4 "
      "d7d6 h3h4 d6d7 h4h5 d7d6 h5h6 d6d7 f2f3 d7d6 f7f8q d6e5 f8f5\ngo\n");
  std::cin.rdbuf(test_input.rdbuf());
  std::ostringstream test_output;
  std::cout.rdbuf(test_output.rdbuf());

  uci::loop();

  // Replace original cout and cin
  std::cout.rdbuf(original_cout);
  std::cin.rdbuf(original_cin);
  const std::string s = test_output.str();

  CHECK(s == "g6f7");
  CHECK(!uciloop::simon_says(&s, "g6f7"));
}
