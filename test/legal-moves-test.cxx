#include <catch2/catch_all.hpp>
#include <iostream>
#include "../src/Board.hpp"
#include "../src/FEN.hpp"

void generate_and_sort(const Board& board, const Square& sq, std::vector<Square>& v)
{
    v = board.legal_moves(sq);
    std::sort(v.begin(), v.end());
}

using s = Square;

Board b;
std::vector<Square> v;

TEST_CASE("gen")
{
    SECTION("pawn excluding promotion") {
        generate_and_sort(b, s::b1, v);
        CHECK(v == std::vector<Square>{s::c3, s::a3});
        generate_and_sort(b, s::b8, v);
        CHECK(v == std::vector<Square>{s::c6, s::a6});
        generate_and_sort(b, s::d1, v);
        CHECK(v == std::vector<Square>{});
        generate_and_sort(b, s::d8, v);
        CHECK(v == std::vector<Square>{});
        generate_and_sort(b, s::a2, v);
        CHECK(v == std::vector<Square>{s::b3, s::a3, s::a4});
        generate_and_sort(b, s::a7, v);
        CHECK(v == std::vector<Square>{s::a5, s::b6, s::a6});
        import_fen(&b, "rnbqkbnr/pppppppp/8/8/8/p7/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        generate_and_sort(b, s::a2, v);
        CHECK(v == std::vector<Square>{s::b3});
        import_fen(&b, "rnbqkbnr/pppppppp/8/8/p7/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        generate_and_sort(b, s::a2, v);
        CHECK(v == std::vector<Square>{s::b3, s::a3});
        import_fen(&b, "rnbqkbnr/pppppppp/P7/1P6/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        generate_and_sort(b, s::a7, v);
        CHECK(v == std::vector<Square>{s::b6});
        generate_and_sort(b, s::b7, v);
        CHECK(v == std::vector<Square>{s::c6, s::b6, s::a6});
    }
}