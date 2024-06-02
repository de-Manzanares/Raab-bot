#include <catch2/catch_all.hpp>
#include <iostream>
#include "../src/Board.hpp"
#include "../src/FEN.hpp"

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
using c = Color;

Board board;
std::vector<Square> v{};

TEST_CASE("capture with king")
{
    SECTION("pawn") {
        import_fen(&board, "8/8/8/3p4/4K3/8/8/8 w - - 0 1");                                        // white king
        generate_and_sort(board, s::e4, v);
        CHECK(v == std::vector<Square>{s::f3, s::e3, s::d3, s::f4, s::d4, s::f5, s::e5, s::d5});
        import_fen(&board, "8/8/8/3p4/2K5/8/8/8 w - - 0 1");
        generate_and_sort(board, s::c4, v);
        CHECK(v == std::vector<Square>{s::d3, s::c3, s::b3, s::d4, s::b4, s::d5, s::c5, s::b5});
        import_fen(&board, "8/8/4k3/3P4/8/8/8/8 w - - 0 1");                                        // black king
        generate_and_sort(board, s::e6, v);
        CHECK(v == std::vector<Square>{s::f5, s::e5, s::d5, s::f6, s::d6, s::f7, s::e7, s::d7});
        import_fen(&board, "8/8/2k5/3P4/8/8/8/8 w - - 0 1");
        generate_and_sort(board, s::c6, v);
        CHECK(v == std::vector<Square>{s::d5, s::c5, s::b5, s::d6, s::b6, s::d7, s::c7, s::b7});
    }SECTION("rook") {
        import_fen(&board, "8/8/8/3r4/3K4/8/8/8 w - - 0 1");                                        // white king
        generate_and_sort(board, s::d4, v);
        CHECK(v == std::vector<Square>{s::e3, s::c3, s::e4, s::c4, s::d5});
        import_fen(&board, "8/8/8/3rK3/8/8/8/8 w - - 0 1");
        generate_and_sort(board, s::e5, v);
        CHECK(v == std::vector<Square>{s::f4, s::e4, s::d5, s::f6, s::e6});
        import_fen(&board, "8/8/3K4/3r4/8/8/8/8 w - - 0 1");
        generate_and_sort(board, s::d6, v);
        CHECK(v == std::vector<Square>{s::d5, s::e6, s::c6, s::e7, s::c7});
        import_fen(&board, "8/8/8/2Kr4/8/8/8/8 w - - 0 1");
        generate_and_sort(board, s::c5, v);
        CHECK(v == std::vector<Square>{s::c4, s::b4, s::d5, s::c6, s::b6});
        import_fen(&board, "8/8/8/3R4/3k4/8/8/8 w - - 0 1");                                        // black king
        generate_and_sort(board, s::d4, v);
        CHECK(v == std::vector<Square>{s::e3, s::c3, s::e4, s::c4, s::d5});
        import_fen(&board, "8/8/8/3Rk3/8/8/8/8 w - - 0 1");
        generate_and_sort(board, s::e5, v);
        CHECK(v == std::vector<Square>{s::f4, s::e4, s::d5, s::f6, s::e6});
        import_fen(&board, "8/8/3k4/3R4/8/8/8/8 w - - 0 1");
        generate_and_sort(board, s::d6, v);
        CHECK(v == std::vector<Square>{s::d5, s::e6, s::c6, s::e7, s::c7});
        import_fen(&board, "8/8/8/2kR4/8/8/8/8 w - - 0 1");
        generate_and_sort(board, s::c5, v);
        CHECK(v == std::vector<Square>{s::c4, s::b4, s::d5, s::c6, s::b6});
    }SECTION("bishop") {
        import_fen(&board, "8/8/8/3b4/2K5/8/8/8 w - - 0 1");                                        // white king
        generate_and_sort(board, s::c4, v);
        CHECK(v == std::vector<Square>{s::d3, s::c3, s::d4, s::b4, s::d5, s::c5, s::b5});
        import_fen(&board, "8/8/8/3b4/4K3/8/8/8 w - - 0 1");
        generate_and_sort(board, s::e4, v);
        CHECK(v == std::vector<Square>{s::e3, s::d3, s::f4, s::d4, s::f5, s::e5, s::d5});
        import_fen(&board, "8/8/4K3/3b4/8/8/8/8 w - - 0 1");
        generate_and_sort(board, s::e6, v);
        CHECK(v == std::vector<Square>{s::f5, s::e5, s::d5, s::f6, s::d6, s::e7, s::d7});
        import_fen(&board, "8/8/2K5/3b4/8/8/8/8 w - - 0 1");
        generate_and_sort(board, s::c6, v);
        CHECK(v == std::vector<Square>{s::d5, s::c5, s::b5, s::d6, s::b6, s::d7, s::c7});
        import_fen(&board, "8/8/8/3B4/2k5/8/8/8 w - - 0 1");                                        // black king
        generate_and_sort(board, s::c4, v);
        CHECK(v == std::vector<Square>{s::d3, s::c3, s::d4, s::b4, s::d5, s::c5, s::b5});
        import_fen(&board, "8/8/8/3B4/4k3/8/8/8 w - - 0 1");
        generate_and_sort(board, s::e4, v);
        CHECK(v == std::vector<Square>{s::e3, s::d3, s::f4, s::d4, s::f5, s::e5, s::d5});
        import_fen(&board, "8/8/4k3/3B4/8/8/8/8 w - - 0 1");
        generate_and_sort(board, s::e6, v);
        CHECK(v == std::vector<Square>{s::f5, s::e5, s::d5, s::f6, s::d6, s::e7, s::d7});
        import_fen(&board, "8/8/2k5/3B4/8/8/8/8 w - - 0 1");
        generate_and_sort(board, s::c6, v);
        CHECK(v == std::vector<Square>{s::d5, s::c5, s::b5, s::d6, s::b6, s::d7, s::c7});
    }
}