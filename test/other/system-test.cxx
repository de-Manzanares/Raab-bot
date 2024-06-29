#include "UCI.h"
#include <catch2/catch_all.hpp>
#include <iostream>

TEST_CASE("https://lichess.org/EKazIsEA") {
  // Save original cout and cin
  std::streambuf *original_cout = std::cout.rdbuf();
  std::streambuf *original_cin = std::cin.rdbuf();

  // Set up input and output streams
  std::istringstream test_input(
      "position startpos moves e2e3 g8f6 b1c3 a7a6 d2d4 d7d5 h2h4 c7c5 d4c5 "
      "d8a5 d1d4 b8c6 d4a4 a5c5 g1f3 c8g4 c1d2 g4f3 g2f3 b7b5 f1b5 f6d7 b5c6 "
      "a8a7 c6d7 a7d7 a4a6 d7a7 a6b5 c5b5 c3b5 a7b7 a2a4 e7e5 d2c3 f7f6 e1c1 "
      "d5d4 e3d4 e5e4 f3e4 f8e7 c3d2 e8d7 h1g1 h8c8 g1g7 b7b8 g7h7 f6f5 h7e7 "
      "d7e7 e4f5 e7d7 d1e1 c8c6 d4d5 c6c8 f5f6 c8e8 e1e8 b8e8 d2e3 e8b8 c1d2 "
      "b8b7 b2b4 b7b8 h4h5 b8b7 h5h6 b7b8 h6h7 b8c8 b5c3 c8f8 c3e4 f8c8 e4c5 "
      "d7d6 c2c4 d6e5 f6f7 e5f5 d2e2 f5g6 d5d6 g6f7 d6d7 c8d8 f2f3 f7g6 a4a5 "
      "g6h7 a5a6 h7g6 a6a7 d8f8 c5e4 f8d8 e4c5 d8f8 c5e4 f8d8 a7a8r d8a8 e4d2 "
      "a8e8 d7e8q\ngo\n");
  std::cin.rdbuf(test_input.rdbuf());
  std::ostringstream test_output;
  std::cout.rdbuf(test_output.rdbuf());

  uci::loop();

  // Replace original cout and cin
  std::cout.rdbuf(original_cout);
  std::cin.rdbuf(original_cin);
  const std::string s = test_output.str();

  CHECK(uciloop::simon_says(&s, "g6f5"));
}
