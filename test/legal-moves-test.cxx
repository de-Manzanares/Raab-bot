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

Board board;
std::vector<Square> v;

TEST_CASE("pawn legal moves")
{
    import_fen(&board, "8/1p1P2p1/P1P2p2/4p3/3P4/2P2p1p/1P2p1P1/8 w - - 0 1");
    generate_and_sort(board, s::g2, v);
    CHECK(v == std::vector<Square>{s::h3, s::g3, s::f3, s::g4});
    generate_and_sort(board, s::e2, v);
    CHECK(v == std::vector<Square>{s::e1});
    generate_and_sort(board, s::b2, v);
    CHECK(v == std::vector<Square>{s::b3, s::b4});
    generate_and_sort(board, s::h3, v);
    CHECK(v == std::vector<Square>{s::h2, s::g2});
    generate_and_sort(board, s::f3, v);
    CHECK(v == std::vector<Square>{s::g2, s::f2});
    generate_and_sort(board, s::f3, v);
    CHECK(v == std::vector<Square>{s::g2, s::f2});
    generate_and_sort(board, s::c3, v);
    CHECK(v == std::vector<Square>{s::c4});
    generate_and_sort(board, s::d4, v);
    CHECK(v == std::vector<Square>{s::e5, s::d5});
    generate_and_sort(board, s::e5, v);
    CHECK(v == std::vector<Square>{s::e4, s::d4});
    generate_and_sort(board, s::f6, v);
    CHECK(v == std::vector<Square>{s::f5});
    generate_and_sort(board, s::c6, v);
    CHECK(v == std::vector<Square>{s::c7, s::b7});
    generate_and_sort(board, s::a6, v);
    CHECK(v == std::vector<Square>{s::b7, s::a7});
    generate_and_sort(board, s::g7, v);
    CHECK(v == std::vector<Square>{s::g5, s::g6});
    generate_and_sort(board, s::d7, v);
    CHECK(v == std::vector<Square>{s::d8});
    generate_and_sort(board, s::b7, v);
    CHECK(v == std::vector<Square>{s::b5, s::c6, s::b6, s::a6});
}