#include <catch2/catch_all.hpp>
#include <iostream>
#include "../src/Board.hpp"
#include "../src/FEN.hpp"

void generate_and_sort(Board& board, const Square& sq, std::vector<Square>& v)
{
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
    SECTION("rook") {
        import_fen(&board, "8/8/3K4/2KrK3/3K4/8/8/8 w - - 0 1");
        generate_and_sort(board, s::d4, v);
        CHECK(v == std::vector<Square>{s::e3, s::c3, s::e4, s::c4, s::d5});
    }
}