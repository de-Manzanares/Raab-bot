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

TEST_CASE("legal moves pawn")
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

TEST_CASE("legal moves rook")
{
    import_fen(&board, "R2R3r/1R6/2R5/R2Rr3/3Rr2r/5r2/6r1/R3r2r w - - 0 1");
    generate_and_sort(board, s::h1, v);
    CHECK(v == std::vector<Square>{s::g1, s::f1, s::h2, s::h3});
    generate_and_sort(board, s::e1, v);
    CHECK(v == std::vector<Square>{s::g1, s::f1, s::d1, s::c1, s::b1, s::a1, s::e2, s::e3});
    generate_and_sort(board, s::a1, v);
    CHECK(v == std::vector<Square>{s::e1, s::d1, s::c1, s::b1, s::a2, s::a3, s::a4});
    generate_and_sort(board, s::g2, v);
    CHECK(v == std::vector<Square>{s::g1, s::h2, s::f2, s::e2, s::d2, s::c2, s::b2, s::a2, s::g3, s::g4, s::g5, s::g6,
                                   s::g7, s::g8});
    generate_and_sort(board, s::f3, v);
    CHECK(v == std::vector<Square>{s::f1, s::f2, s::h3, s::g3, s::e3, s::d3, s::c3, s::b3, s::a3, s::f4, s::f5, s::f6,
                                   s::f7, s::f8});
    generate_and_sort(board, s::h4, v);
    CHECK(v == std::vector<Square>{s::h2, s::h3, s::g4, s::f4, s::h5, s::h6, s::h7});
    generate_and_sort(board, s::e4, v);
    CHECK(v == std::vector<Square>{s::e2, s::e3, s::g4, s::f4, s::d4});
    generate_and_sort(board, s::d4, v);
    CHECK(v == std::vector<Square>{s::d1, s::d2, s::d3, s::e4, s::c4, s::b4, s::a4});
    generate_and_sort(board, s::e5, v);
    CHECK(v == std::vector<Square>{s::h5, s::g5, s::f5, s::d5, s::e6, s::e7, s::e8});
    generate_and_sort(board, s::d5, v);
    CHECK(v == std::vector<Square>{s::e5, s::c5, s::b5, s::d6, s::d7});
    generate_and_sort(board, s::a5, v);
    CHECK(v == std::vector<Square>{s::a2, s::a3, s::a4, s::c5, s::b5, s::a6, s::a7});
    generate_and_sort(board, s::c6, v);
    CHECK(v == std::vector<Square>{s::c1, s::c2, s::c3, s::c4, s::c5, s::h6, s::g6, s::f6, s::e6, s::d6, s::b6, s::a6,
                                   s::c7, s::c8});
    generate_and_sort(board, s::b7, v);
    CHECK(v == std::vector<Square>{s::b1, s::b2, s::b3, s::b4, s::b5, s::b6, s::h7, s::g7, s::f7, s::e7, s::d7, s::c7,
                                   s::a7, s::b8});
    generate_and_sort(board, s::h8, v);
    CHECK(v == std::vector<Square>{s::h5, s::h6, s::h7, s::g8, s::f8, s::e8, s::d8});
    generate_and_sort(board, s::d8, v);
    CHECK(v == std::vector<Square>{s::d6, s::d7, s::h8, s::g8, s::f8, s::e8, s::c8, s::b8});
    generate_and_sort(board, s::a8, v);
    CHECK(v == std::vector<Square>{s::a6, s::a7, s::c8, s::b8});
}
