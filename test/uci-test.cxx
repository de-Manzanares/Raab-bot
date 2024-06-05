#include <catch2/catch_all.hpp>
#include <iostream>
#include <sstream>
#include "../src/uci.hpp"

TEST_CASE("bruh")
{
    // Save original cout and cin
    std::streambuf* original_cout = std::cout.rdbuf();
    std::streambuf* original_cin = std::cin.rdbuf();

    // Set up input and output streams
    std::istringstream test_input("position startpos moves e2e4 e7e5\ngo\n"); // replace with the input for the test
    std::cin.rdbuf(test_input.rdbuf());
    std::ostringstream test_output;
    std::cout.rdbuf(test_output.rdbuf());

    uci::loop();

    // Replace original cout and cin
    std::cout.rdbuf(original_cout);
    std::cin.rdbuf(original_cin);

    CHECK(test_output.str() == "e");
}