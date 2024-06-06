#include <catch2/catch_all.hpp>
#include <iostream>
#include "../src/Board.hpp"
#include "../src/tree.hpp"
#include "../src/uci.hpp"

void generate_and_sort(Board& board, const Square& sq, std::vector<Square>& v)
{
    v.clear();
    board.update_move_maps();
    Color c = board.what_color(sq);
    if (c == Color::white) { v = board.move_map_white[sq]; }
    if (c == Color::black) { v = board.move_map_black[sq]; }
    std::sort(v.begin(), v.end());
}

using s = Square;

Board board;
std::vector<Square> v;

TEST_CASE("en passant")
{
    SECTION("single target white") {
        board.import_fen("4k3/8/8/8/2Pp4/8/8/4K3 w - - 0 1");                 // no target
        generate_and_sort(board, s::d4, v);
        CHECK(v == std::vector<Square>{s::d3});
        board.import_fen("4k3/8/8/8/2Pp4/8/8/4K3 w - c3 0 1");                // target
        generate_and_sort(board, s::d4, v);
        CHECK(v == std::vector<Square>{s::d3, s::c3});
    }SECTION("single target black") {
        board.import_fen("4k3/8/8/2Pp4/8/8/8/4K3 w - d6 0 1");
        generate_and_sort(board, s::c5, v);
        CHECK(v == std::vector<Square>{s::d6, s::c6});
    }SECTION("double target white") {
        board.import_fen("4k3/8/8/8/1pPp4/8/8/4K3 w - c3 0 1");
        generate_and_sort(board, s::d4, v);
        CHECK(v == std::vector<Square>{s::d3, s::c3});
        generate_and_sort(board, s::b4, v);
        CHECK(v == std::vector<Square>{s::c3, s::b3});
    }SECTION("double target black") {
        board.import_fen("4k3/8/8/2PpP3/8/8/8/4K3 w - d6 0 1");
        generate_and_sort(board, s::e5, v);
        CHECK(v == std::vector<Square>{s::e6, s::d6});
        generate_and_sort(board, s::c5, v);
        CHECK(v == std::vector<Square>{s::d6, s::c6});
    }SECTION("remove en passant pawn") {
        board.import_fen("8/8/2Pp4/6K1/8/8/2k5/8 w - d7 0 1");
        board.move(s::c6, s::d7, 0);
        CHECK(board.is_empty(Square::d6));
    }
}

TEST_CASE(
        "2024-06-06 02:12:56,986 lib.engine_wrapper (engine_wrapper.py:185) ERROR Ending game due to bot attempting an illegal move.")
{
    // en passant error

    // Save original cout and cin
    std::streambuf *original_cout = std::cout.rdbuf();
    std::streambuf *original_cin = std::cin.rdbuf();

    // Set up input and output streams
    std::istringstream test_input(
            "position startpos moves e2e4 e7e6 f1b5 a7a6 b5c4 d7d5 e4d5 e6d5 c4d3 b8c6 d1e2 e8d7 b1c3 d8h4 c3d5 d7d8 g2g3 h4d4 d5f4 c8d7 g1f3 d4b4 e1g1 g8f6 c2c3 b4c5 b2b4 c5d6 c1b2 h7h6 f3h4 c6a7 f1e1 a7c8 a2a4 d7c6 f2f3 c6d7 a4a5 a8a7 h2h3 a7a8 d3c4 c8a7 c4f7 g7g5 h4g6 g5f4 g6h8 f4g3 h3h4 d7f5 d2d4 d6c6 h4h5 a7c8 f3f4 c8d6 d4d5 c6d7 h8g6 d7f7 g6f8 f7f8 c3c4 f6d7 c4c5 d6f7 a1a4 b7b5 a5b6\ngo\n");
    std::cin.rdbuf(test_input.rdbuf());
    std::ostringstream test_output;
    std::cout.rdbuf(test_output.rdbuf());

    uci::loop();

    // Replace original cout and cin
    std::cout.rdbuf(original_cout);
    std::cin.rdbuf(original_cin);

    CHECK(test_output.str() != "bestmove b5a4");
}

TEST_CASE("pinned pieces rook")
{
    board.import_fen("8/8/8/1k1pR2K/8/8/8/8 w - - 0 1");
    CHECK(board.pinned_piece_rook(s::e5) == s::d5);
}
