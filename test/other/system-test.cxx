#include "UCI.h"
#include <catch2/catch_all.hpp>
#include <iostream>

TEST_CASE("system test") {
  // Save original cout and cin
  std::streambuf *original_cout = std::cout.rdbuf();
  std::streambuf *original_cin = std::cin.rdbuf();

  // Set up input and output streams
  std::istringstream test_input("position startpos moves b1a3 e7e6 f2f4 d7d5 "
                                "g1f3 g8f6 e2e3 a7a6 f1e2 b8c6\ngo\n");
  std::cin.rdbuf(test_input.rdbuf());
  std::ostringstream test_output;
  std::cout.rdbuf(test_output.rdbuf());

  uci::loop();

  // Replace original cout and cin
  std::cout.rdbuf(original_cout);
  std::cin.rdbuf(original_cin);

  CHECK(test_output.str() == "e");
}
