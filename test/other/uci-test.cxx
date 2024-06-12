#include <catch2/catch_all.hpp>
#include <iostream>
#include <sstream>
#include "../../src/UCI.cpp"

TEST_CASE("bruh")
{
    // Save original cout and cin
    std::streambuf *original_cout = std::cout.rdbuf();
    std::streambuf *original_cin = std::cin.rdbuf();

    // Set up input and output streams
    std::istringstream test_input(
            "position startpos moves e2e4 e7e5 g1f3 d7d5 e4d5 h7h5 d2d4 g7g6 g2g4 a7a5 b1c3 b7b6\ngo");
    std::cin.rdbuf(test_input.rdbuf());
    std::ostringstream test_output;
    std::cout.rdbuf(test_output.rdbuf());

    uci::loop();

    // Replace original cout and cin
    std::cout.rdbuf(original_cout);
    std::cin.rdbuf(original_cin);

    CHECK(test_output.str() == "e");
}
