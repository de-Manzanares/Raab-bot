#include <catch2/catch_all.hpp>
#include <iostream>
#include "../../include/UCI.h"

void generate_and_sort_omt(Board& board, const Square& sq, std::vector<Square>& v)
{
    v.clear();
    board.update_move_maps();
    Color c = board.what_color(sq);
    if (c == Color::white) { v = board.maps->move_map_white[sq]; }
    if (c == Color::black) { v = board.maps->move_map_black[sq]; }
    std::sort(v.begin(), v.end());
}

using s = Square;

Board board_omt;
std::vector<Square> v_omt;

TEST_CASE("en passant")
{
    SECTION("single target white") {
        board_omt.import_fen("4k3/8/8/8/2Pp4/8/8/4K3 w - - 0 1");                 // no target
        generate_and_sort_omt(board_omt, s::d4, v_omt);
        CHECK(v_omt == std::vector<Square>{s::d3});
        board_omt.import_fen("4k3/8/8/8/2Pp4/8/8/4K3 w - c3 0 1");                // target
        generate_and_sort_omt(board_omt, s::d4, v_omt);
        CHECK(v_omt == std::vector<Square>{s::d3, s::c3});
    }SECTION("single target black") {
        board_omt.import_fen("4k3/8/8/2Pp4/8/8/8/4K3 w - d6 0 1");
        generate_and_sort_omt(board_omt, s::c5, v_omt);
        CHECK(v_omt == std::vector<Square>{s::d6, s::c6});
    }SECTION("double target white") {
        board_omt.import_fen("4k3/8/8/8/1pPp4/8/8/4K3 w - c3 0 1");
        generate_and_sort_omt(board_omt, s::d4, v_omt);
        CHECK(v_omt == std::vector<Square>{s::d3, s::c3});
        generate_and_sort_omt(board_omt, s::b4, v_omt);
        CHECK(v_omt == std::vector<Square>{s::c3, s::b3});
    }SECTION("double target black") {
        board_omt.import_fen("4k3/8/8/2PpP3/8/8/8/4K3 w - d6 0 1");
        generate_and_sort_omt(board_omt, s::e5, v_omt);
        CHECK(v_omt == std::vector<Square>{s::e6, s::d6});
        generate_and_sort_omt(board_omt, s::c5, v_omt);
        CHECK(v_omt == std::vector<Square>{s::d6, s::c6});
    }SECTION("remove en passant pawn") {
        board_omt.import_fen("8/8/2Pp4/6K1/8/8/2k5/8 w - d7 0 1");
        board_omt.move(s::c6, s::d7, 0);
        CHECK(board_omt.is_empty(Square::d6));
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

TEST_CASE(
        "2024-06-07 06:52:36,392 lib.engine_wrapper (engine_wrapper.py:185) ERROR Ending game due to bot attempting an illegal move.")
{
    // castling error

    // Save original cout and cin
    std::streambuf *original_cout = std::cout.rdbuf();
    std::streambuf *original_cin = std::cin.rdbuf();

    // Set up input and output streams
    std::istringstream test_input(
            "position startpos moves g2g4 g8f6 g4g5 f6d5 f1g2 c7c6 e2e4 d5f4 g2f3 e7e5 f3g4 d8g5 g4d7 b8d7 g1f3 g5g2 f3g1 g2h1 d1g4 h7h5 g4g5 f4h3 g5g3 h1g1 g3g1 h3g1\ngo\n");
    std::cin.rdbuf(test_input.rdbuf());
    std::ostringstream test_output;
    std::cout.rdbuf(test_output.rdbuf());

    uci::loop();

    // Replace original cout and cin
    std::cout.rdbuf(original_cout);
    std::cin.rdbuf(original_cin);

    CHECK(test_output.str() != "bestmove e1g1");
}

TEST_CASE(
        "2024-06-08 06:30:23,327 lib.engine_wrapper (engine_wrapper.py:185) ERROR Ending game due to bot attempting an illegal move.")
{
    // castling error
    // when castling long, you have to check that the third square is not occupied.

    // Save original cout and cin
    std::streambuf *original_cout = std::cout.rdbuf();
    std::streambuf *original_cin = std::cin.rdbuf();

    // Set up input and output streams
    std::istringstream test_input(
            "position startpos moves d2d4 d7d5 c2c4 d8d6 c4d5 d6d5 d1a4 c8d7 a4b4 b8c6 b4b7 a8b8 b7c7 d5a5 c7a5 c6a5 c1d2 a5c4 d2c3 g8f6 e2e3 c4b2 a2a4 e7e5 d4e5 f6e4 e5e6 f7e6 c3b2 b8b2\ngo\n");
    std::cin.rdbuf(test_input.rdbuf());
    std::ostringstream test_output;
    std::cout.rdbuf(test_output.rdbuf());

    uci::loop();

    // Replace original cout and cin
    std::cout.rdbuf(original_cout);
    std::cin.rdbuf(original_cin);

    CHECK(test_output.str() != "bestmove e1b1");
}

TEST_CASE(
        "2024-06-08 03:17:07,256 lib.engine_wrapper (engine_wrapper.py:185) ERROR Ending game due to bot attempting an illegal move.")
{
    // castling error
    // when castling long, you have to check that the third square is not occupied.

    // Save original cout and cin
    std::streambuf *original_cout = std::cout.rdbuf();
    std::streambuf *original_cin = std::cin.rdbuf();

    // Set up input and output streams
    std::istringstream test_input(
            "position startpos moves b1c3 g8f6 g1f3 d7d5 d2d4 a7a6 c1f4 b7b6 f3e5 e7e6 a2a4 f8b4 g2g4 d8d6 e5f7 b4c3 b2c3 d6f4 f7h8\ngo\n");
    std::cin.rdbuf(test_input.rdbuf());
    std::ostringstream test_output;
    std::cout.rdbuf(test_output.rdbuf());

    uci::loop();

    // Replace original cout and cin
    std::cout.rdbuf(original_cout);
    std::cin.rdbuf(original_cin);

    CHECK(test_output.str() != "e8g8");
}

TEST_CASE("pinned pieces rook")
{
    board_omt.import_fen("8/8/8/1k1pR2K/8/8/8/8 w - - 0 1");
    CHECK(board_omt.pinned_piece_rook(s::e5) == s::d5);
}

TEST_CASE("https://lichess.org/moafHuhpD6Fi attempting d5d4")
{
    // pawn taken in en passant is not removed?

    // Save original cout and cin
    std::streambuf *original_cout = std::cout.rdbuf();
    std::streambuf *original_cin = std::cin.rdbuf();

    // Set up input and output streams
    std::istringstream test_input(
            "position startpos moves c2c4 g8f6 b1c3 e7e6 d1a4 c7c5 g1f3 f8e7 e2e4 b8c6 f1d3 c6b4 d3e2 b4c6 e1g1 e8g8 e4e5 f6g4 c3b5 d8b6 e2d3 c6b4 d3e4 d7d5 e5d6 e7f6 h2h3 g4h6 a2a3 b4a6 d2d4 c5d4 c1h6 g7h6 a4c2 d4d3 e4d3 g8g7 a1d1 c8d7 d3e2 b6c6 e2d3 a6c5 b2b4 c5d3 c2d3 a7a6 f3d4 c6b6 d3g3 g7h8 g3f4 f6d4 b5d4 f7f6 c4c5 b6a7 f4h6 e6e5 d4f3 e5e4 f3h4 h8g8 f1e1 d7a4 d1d2 a8e8 d2d4 a4d7 d4e4 e8e4 e1e4 f8f7 e4f4 f7f8 f4f6 f8f6 h6f6 a7b8 f6g5 g8f7 g5e7 f7g8 e7d7 g8h8 d7e7 b8c8 d6d7 c8b8 d7d8q b8d8 e7d8 h8g7 d8d7 g7g8 h4f5 g8f8 f5h6\n");
    std::cin.rdbuf(test_input.rdbuf());
    std::ostringstream test_output;
    std::cout.rdbuf(test_output.rdbuf());

    uci::loop();

    // Replace original cout and cin
    std::cout.rdbuf(original_cout);
    std::cin.rdbuf(original_cin);

    CHECK(test_output.str() != "d5d4");
}

TEST_CASE("https://lichess.org/VURi7gATBNpp attempting c5d4")
{
    // pawn taken in en passant is not removed?

    // Save original cout and cin
    std::streambuf *original_cout = std::cout.rdbuf();
    std::streambuf *original_cin = std::cin.rdbuf();

    // Set up input and output streams
    std::istringstream test_input(
            "position startpos moves c2c4 e7e5 b1c3 g8f6 g1f3 b8c6 e2e4 f8b4 f1d3 d7d6 a2a3 b4c5 b2b4 c5b6 h2h3 c8e6 d1a4 e8e7 c1b2 a7a6 a1d1 d8d7 c3d5 f6d5 c4d5 c6d4 a4d7 e6d7 f3d4 b6d4 b2d4 e5d4 d1c1 e7d8 c1c4 d7b5 c4d4 c7c5 d5c6 b5d3 d4d3 d8c7 c6b7 a8e8 e1g1 e8e4 d3f3 f7f6 f3g3 g7g5 g3e3 e4e3 f2e3 h8f8 f1f5 c7b7 f5d5 f8e8 d5d6 b7b8 d6f6 e8d8 d2d4 d8e8 g1f2 g5g4 h3g4 a6a5 b4a5 b8a8 a5a6 a8a7 a3a4 e8e7 a4a5 e7e8 f6c6 e8e7 f2f3 e7e8 e3e4 e8e7 e4e5 e7d7 f3e4 d7e7 e5e6 h7h6 e4f5 e7g7 f5f4 g7e7 c6d6 e7g7 d6d7 g7d7 e6d7 a7a6 d7d8q a6b5 f4f5\ngo\n");
    std::cin.rdbuf(test_input.rdbuf());
    std::ostringstream test_output;
    std::cout.rdbuf(test_output.rdbuf());

    uci::loop();

    // Replace original cout and cin
    std::cout.rdbuf(original_cout);
    std::cin.rdbuf(original_cin);
    std::string string = test_output.str();
    std::cout << string;
    CHECK(!simon_says(&string, "c5d4"));
}

TEST_CASE("https://lichess.org/f3rXnbLtD71N attempting f4g5")
{
    // pawn taken in en passant is not removed?

    // Save original cout and cin
    std::streambuf *original_cout = std::cout.rdbuf();
    std::streambuf *original_cin = std::cin.rdbuf();

    // Set up input and output streams
    std::istringstream test_input(
            "position startpos moves e2e4 b8c6 g1f3 c6b4 d2d4 g8f6 f3g5 h7h6 c1d2 h6g5 d2b4 e7e6 b4f8 e8f8 f1d3 c7c6 d1d2 f6h5 d2b4 d8e7 b4a5 b7b6 a5a4 h5f4 d3f1 b6b5 a4a5 g5g4 g2g3 f4g6 b1d2 d7d5 e4e5 f7f5 f1g2 a8b8 e1e2 f5f4 a1f1 f4f3 d2f3 g4f3 g2f3 a7a6 f1a1 f8f7 f3g4 e7a7 a5b4 c6c5 d4c5 c8d7 b4d4 b5b4 e2e3 f7e7 h2h4 a6a5 h4h5 g6f8 g4e2 d7c6 d4h4 e7e8 h4d4 a7e7 f2f4 c6a4 b2b3 a4c6 e3f2 e7d8 f2e3 g7g5 h5g6 h8h1 g6g7 h1a1 g7g8q a1a2 e3d2 c6b5 e2b5 b8b5\ngo\n");
    std::cin.rdbuf(test_input.rdbuf());
    std::ostringstream test_output;
    std::cout.rdbuf(test_output.rdbuf());

    uci::loop();

    // Replace original cout and cin
    std::cout.rdbuf(original_cout);
    std::cin.rdbuf(original_cin);
    std::string string = test_output.str();
    std::cout << string;
    CHECK(!simon_says(&string, "f4g5"));
}
