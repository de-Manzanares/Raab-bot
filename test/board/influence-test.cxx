#include "../../include/Board.h"
#include <catch2/catch_all.hpp>

void generate_and_sort_influence(Board &board, const Square &sq,
                                 std::vector<Square> &v) {
  board.update_influence_maps();
  Color c = board.what_color(sq);
  if (c == Color::white) {
    v = board.maps->white_influence[sq];
  }
  if (c == Color::black) {
    v = board.maps->black_influence[sq];
  }
  std::sort(v.begin(), v.end());
}

using s = Square;
Board board_i;
std::vector<Square> v_i;

TEST_CASE("rook attack map") {
  board_i.import_fen("R2R3R/1R6/2R5/R2RR3/3RR2R/5R2/6R1/R3R2R w - - 0 1");
  generate_and_sort_influence(board_i, s::h1, v_i);
  CHECK(v_i == std::vector<Square>{s::g1, s::f1, s::e1, s::h2, s::h3, s::h4});
  generate_and_sort_influence(board_i, s::e1, v_i);
  CHECK(v_i == std::vector<Square>{s::h1, s::g1, s::f1, s::d1, s::c1, s::b1,
                                   s::a1, s::e2, s::e3, s::e4});
  generate_and_sort_influence(board_i, s::a1, v_i);
  CHECK(v_i == std::vector<Square>{s::e1, s::d1, s::c1, s::b1, s::a2, s::a3,
                                   s::a4, s::a5});
  generate_and_sort_influence(board_i, s::g2, v_i);
  CHECK(v_i == std::vector<Square>{s::g1, s::h2, s::f2, s::e2, s::d2, s::c2,
                                   s::b2, s::a2, s::g3, s::g4, s::g5, s::g6,
                                   s::g7, s::g8});
  generate_and_sort_influence(board_i, s::f3, v_i);
  CHECK(v_i == std::vector<Square>{s::f1, s::f2, s::h3, s::g3, s::e3, s::d3,
                                   s::c3, s::b3, s::a3, s::f4, s::f5, s::f6,
                                   s::f7, s::f8});
  generate_and_sort_influence(board_i, s::h4, v_i);
  CHECK(v_i == std::vector<Square>{s::h1, s::h2, s::h3, s::g4, s::f4, s::e4,
                                   s::h5, s::h6, s::h7, s::h8});
  generate_and_sort_influence(board_i, s::e4, v_i);
  CHECK(v_i == std::vector<Square>{s::e1, s::e2, s::e3, s::h4, s::g4, s::f4,
                                   s::d4, s::e5});
  generate_and_sort_influence(board_i, s::d4, v_i);
  CHECK(v_i == std::vector<Square>{s::d1, s::d2, s::d3, s::e4, s::c4, s::b4,
                                   s::a4, s::d5});
  generate_and_sort_influence(board_i, s::e5, v_i);
  CHECK(v_i == std::vector<Square>{s::e4, s::h5, s::g5, s::f5, s::d5, s::e6,
                                   s::e7, s::e8});
  generate_and_sort_influence(board_i, s::d5, v_i);
  CHECK(v_i == std::vector<Square>{s::d4, s::e5, s::c5, s::b5, s::a5, s::d6,
                                   s::d7, s::d8});
  generate_and_sort_influence(board_i, s::a5, v_i);
  CHECK(v_i == std::vector<Square>{s::a1, s::a2, s::a3, s::a4, s::d5, s::c5,
                                   s::b5, s::a6, s::a7, s::a8});
  generate_and_sort_influence(board_i, s::c6, v_i);
  CHECK(v_i == std::vector<Square>{s::c1, s::c2, s::c3, s::c4, s::c5, s::h6,
                                   s::g6, s::f6, s::e6, s::d6, s::b6, s::a6,
                                   s::c7, s::c8});
  generate_and_sort_influence(board_i, s::b7, v_i);
  CHECK(v_i == std::vector<Square>{s::b1, s::b2, s::b3, s::b4, s::b5, s::b6,
                                   s::h7, s::g7, s::f7, s::e7, s::d7, s::c7,
                                   s::a7, s::b8});
  generate_and_sort_influence(board_i, s::h8, v_i);
  CHECK(v_i == std::vector<Square>{s::h4, s::h5, s::h6, s::h7, s::g8, s::f8,
                                   s::e8, s::d8});
  generate_and_sort_influence(board_i, s::d8, v_i);
  CHECK(v_i == std::vector<Square>{s::d5, s::d6, s::d7, s::h8, s::g8, s::f8,
                                   s::e8, s::c8, s::b8, s::a8});
  generate_and_sort_influence(board_i, s::a8, v_i);
  CHECK(v_i == std::vector<Square>{s::a5, s::a6, s::a7, s::d8, s::c8, s::b8});
}

TEST_CASE("bishop attack map") {
  board_i.import_fen("B2B3B/3B4/3B4/3BB2B/B2BB3/4B3/4B3/B3B2B w - - 0 1");
  generate_and_sort_influence(board_i, s::h1, v_i);
  CHECK(v_i == std::vector<Square>{s::g2, s::f3, s::e4});
  generate_and_sort_influence(board_i, s::e1, v_i);
  CHECK(v_i ==
        std::vector<Square>{s::f2, s::d2, s::g3, s::c3, s::h4, s::b4, s::a5});
  generate_and_sort_influence(board_i, s::a1, v_i);
  CHECK(v_i == std::vector<Square>{s::b2, s::c3, s::d4});
  generate_and_sort_influence(board_i, s::e2, v_i);
  CHECK(v_i == std::vector<Square>{s::f1, s::d1, s::f3, s::d3, s::g4, s::c4,
                                   s::h5, s::b5, s::a6});
  generate_and_sort_influence(board_i, s::e3, v_i);
  CHECK(v_i == std::vector<Square>{s::g1, s::c1, s::f2, s::d2, s::f4, s::d4,
                                   s::g5, s::h6});
  generate_and_sort_influence(board_i, s::e4, v_i);
  CHECK(v_i == std::vector<Square>{s::h1, s::b1, s::g2, s::c2, s::f3, s::d3,
                                   s::f5, s::d5, s::g6, s::h7});
  generate_and_sort_influence(board_i, s::d4, v_i);
  CHECK(v_i == std::vector<Square>{s::a1, s::b2, s::e3, s::c3, s::e5, s::c5,
                                   s::b6, s::a7});
  generate_and_sort_influence(board_i, s::a4, v_i);
  CHECK(v_i == std::vector<Square>{s::d1, s::c2, s::b3, s::b5, s::c6, s::d7});
  generate_and_sort_influence(board_i, s::h5, v_i);
  CHECK(v_i == std::vector<Square>{s::e2, s::f3, s::g4, s::g6, s::f7, s::e8});
  generate_and_sort_influence(board_i, s::e5, v_i);
  CHECK(v_i == std::vector<Square>{s::h2, s::g3, s::f4, s::d4, s::f6, s::d6,
                                   s::g7, s::h8});
  generate_and_sort_influence(board_i, s::d5, v_i);
  CHECK(v_i == std::vector<Square>{s::a2, s::b3, s::e4, s::c4, s::e6, s::c6,
                                   s::f7, s::b7, s::g8, s::a8});
  generate_and_sort_influence(board_i, s::d6, v_i);
  CHECK(v_i == std::vector<Square>{s::a3, s::b4, s::e5, s::c5, s::e7, s::c7,
                                   s::f8, s::b8});
  generate_and_sort_influence(board_i, s::d7, v_i);
  CHECK(v_i == std::vector<Square>{s::h3, s::g4, s::a4, s::f5, s::b5, s::e6,
                                   s::c6, s::e8, s::c8});
  generate_and_sort_influence(board_i, s::h8, v_i);
  CHECK(v_i == std::vector<Square>{s::e5, s::f6, s::g7});
  generate_and_sort_influence(board_i, s::d8, v_i);
  CHECK(v_i ==
        std::vector<Square>{s::h4, s::g5, s::a5, s::f6, s::b6, s::e7, s::c7});
  generate_and_sort_influence(board_i, s::a8, v_i);
  CHECK(v_i == std::vector<Square>{s::d5, s::c6, s::b7});
}

TEST_CASE("queen attack map") {
  board_i.import_fen("Q2Q3Q/8/8/Q2Q4/4Q2Q/8/8/Q3Q2Q w - - 0 1");
  generate_and_sort_influence(board_i, s::h1, v_i);
  CHECK(v_i == std::vector<Square>{s::g1, s::f1, s::e1, s::h2, s::g2, s::h3,
                                   s::f3, s::h4, s::e4});
  generate_and_sort_influence(board_i, s::e1, v_i);
  CHECK(v_i == std::vector<Square>{s::h1, s::g1, s::f1, s::d1, s::c1, s::b1,
                                   s::a1, s::f2, s::e2, s::d2, s::g3, s::e3,
                                   s::c3, s::h4, s::e4, s::b4, s::a5});
  generate_and_sort_influence(board_i, s::a1, v_i);
  CHECK(v_i == std::vector<Square>{s::e1, s::d1, s::c1, s::b1, s::b2, s::a2,
                                   s::c3, s::a3, s::d4, s::a4, s::e5, s::a5,
                                   s::f6, s::g7, s::h8});
  generate_and_sort_influence(board_i, s::h4, v_i);
  CHECK(v_i == std::vector<Square>{s::h1, s::e1, s::h2, s::f2, s::h3, s::g3,
                                   s::g4, s::f4, s::e4, s::h5, s::g5, s::h6,
                                   s::f6, s::h7, s::e7, s::h8, s::d8});
  generate_and_sort_influence(board_i, s::e4, v_i);
  CHECK(v_i == std::vector<Square>{s::h1, s::e1, s::b1, s::g2, s::e2, s::c2,
                                   s::f3, s::e3, s::d3, s::h4, s::g4, s::f4,
                                   s::d4, s::c4, s::b4, s::a4, s::f5, s::e5,
                                   s::d5, s::g6, s::e6, s::h7, s::e7, s::e8});
  generate_and_sort_influence(board_i, s::d5, v_i);
  CHECK(v_i == std::vector<Square>{s::d1, s::d2, s::a2, s::d3, s::b3, s::e4,
                                   s::d4, s::c4, s::h5, s::g5, s::f5, s::e5,
                                   s::c5, s::b5, s::a5, s::e6, s::d6, s::c6,
                                   s::f7, s::d7, s::b7, s::g8, s::d8, s::a8});
  generate_and_sort_influence(board_i, s::a5, v_i);
  CHECK(v_i == std::vector<Square>{s::e1, s::a1, s::d2, s::a2, s::c3, s::a3,
                                   s::b4, s::a4, s::d5, s::c5, s::b5, s::b6,
                                   s::a6, s::c7, s::a7, s::d8, s::a8});
  generate_and_sort_influence(board_i, s::h8, v_i);
  CHECK(v_i == std::vector<Square>{s::a1, s::b2, s::c3, s::h4, s::d4, s::h5,
                                   s::e5, s::h6, s::f6, s::h7, s::g7, s::g8,
                                   s::f8, s::e8, s::d8});
  generate_and_sort_influence(board_i, s::d8, v_i);
  CHECK(v_i == std::vector<Square>{s::h4, s::g5, s::d5, s::a5, s::f6, s::d6,
                                   s::b6, s::e7, s::d7, s::c7, s::h8, s::g8,
                                   s::f8, s::e8, s::c8, s::b8, s::a8});
  generate_and_sort_influence(board_i, s::a8, v_i);
  CHECK(v_i == std::vector<Square>{s::d5, s::a5, s::c6, s::a6, s::b7, s::a7,
                                   s::d8, s::c8, s::b8});
}

TEST_CASE("attack map knight") {
  board_i.import_fen(
      "NN1N2NN/NN2N1NN/8/N3N1N1/1N5N/8/NN1N2NN/NN2N1NN w - - 0 1");
  // center 4x4
  generate_and_sort_influence(board_i, s::e5, v_i);
  CHECK(v_i == std::vector<Square>{s::f3, s::d3, s::g4, s::c4, s::g6, s::c6,
                                   s::f7, s::d7});
  // b file - 4 squares
  generate_and_sort_influence(board_i, s::b4, v_i);
  CHECK(v_i == std::vector<Square>{s::c2, s::a2, s::d3, s::d5, s::c6, s::a6});
  // g files - 4 squares
  generate_and_sort_influence(board_i, s::g5, v_i);
  CHECK(v_i == std::vector<Square>{s::h3, s::f3, s::e4, s::e6, s::h7, s::f7});
  // 2nd rank - 4 squares
  generate_and_sort_influence(board_i, s::d2, v_i);
  CHECK(v_i == std::vector<Square>{s::f1, s::b1, s::f3, s::b3, s::e4, s::c4});
  // 7th rank - 4 squares
  generate_and_sort_influence(board_i, s::e7, v_i);
  CHECK(v_i == std::vector<Square>{s::f5, s::d5, s::g6, s::c6, s::g8, s::c8});
  // a file - 4 squares
  generate_and_sort_influence(board_i, s::a5, v_i);
  CHECK(v_i == std::vector<Square>{s::b3, s::c4, s::c6, s::b7});
  // h file - 4 squares
  generate_and_sort_influence(board_i, s::h4, v_i);
  CHECK(v_i == std::vector<Square>{s::g2, s::f3, s::f5, s::g6});
  // 1st rank - 4 squares
  generate_and_sort_influence(board_i, s::e1, v_i);
  CHECK(v_i == std::vector<Square>{s::g2, s::c2, s::f3, s::d3});
  // 8th rank - 4 squares
  generate_and_sort_influence(board_i, s::d8, v_i);
  CHECK(v_i == std::vector<Square>{s::e6, s::c6, s::f7, s::b7});
  // corners
  generate_and_sort_influence(board_i, s::h1, v_i);
  CHECK(v_i == std::vector<Square>{s::f2, s::g3});
  generate_and_sort_influence(board_i, s::a1, v_i);
  CHECK(v_i == std::vector<Square>{s::c2, s::b3});
  generate_and_sort_influence(board_i, s::h8, v_i);
  CHECK(v_i == std::vector<Square>{s::g6, s::f7});
  generate_and_sort_influence(board_i, s::a8, v_i);
  CHECK(v_i == std::vector<Square>{s::b6, s::c7});
  // inside 2's and 7's
  generate_and_sort_influence(board_i, s::g2, v_i);
  CHECK(v_i == std::vector<Square>{s::e1, s::e3, s::h4, s::f4});
  generate_and_sort_influence(board_i, s::b2, v_i);
  CHECK(v_i == std::vector<Square>{s::d1, s::d3, s::c4, s::a4});
  generate_and_sort_influence(board_i, s::g7, v_i);
  CHECK(v_i == std::vector<Square>{s::h5, s::f5, s::e6, s::e8});
  generate_and_sort_influence(board_i, s::b7, v_i);
  CHECK(v_i == std::vector<Square>{s::c5, s::a5, s::d6, s::d8});
  // edge 1's and 8's
  generate_and_sort_influence(board_i, s::g1, v_i);
  CHECK(v_i == std::vector<Square>{s::e2, s::h3, s::f3});
  generate_and_sort_influence(board_i, s::b1, v_i);
  CHECK(v_i == std::vector<Square>{s::d2, s::c3, s::a3});
  generate_and_sort_influence(board_i, s::g8, v_i);
  CHECK(v_i == std::vector<Square>{s::h6, s::f6, s::e7});
  generate_and_sort_influence(board_i, s::b8, v_i);
  CHECK(v_i == std::vector<Square>{s::c6, s::a6, s::d7});
  // edge 2's and 7's
  generate_and_sort_influence(board_i, s::h2, v_i);
  CHECK(v_i == std::vector<Square>{s::f1, s::f3, s::g4});
  generate_and_sort_influence(board_i, s::a2, v_i);
  CHECK(v_i == std::vector<Square>{s::c1, s::c3, s::b4});
  generate_and_sort_influence(board_i, s::h7, v_i);
  CHECK(v_i == std::vector<Square>{s::g5, s::f6, s::f8});
  generate_and_sort_influence(board_i, s::a7, v_i);
  CHECK(v_i == std::vector<Square>{s::b5, s::c6, s::c8});
}

TEST_CASE("king attack map") {
  board_i.import_fen("K2K3K/8/8/K5K1/1K2K2K/8/8/K3K2K w - - 0 1");
  generate_and_sort_influence(board_i, s::h1, v_i);
  CHECK(v_i == std::vector<Square>{s::g1, s::h2, s::g2});
  generate_and_sort_influence(board_i, s::e1, v_i);
  CHECK(v_i == std::vector<Square>{s::f1, s::d1, s::f2, s::e2, s::d2});
  generate_and_sort_influence(board_i, s::a1, v_i);
  CHECK(v_i == std::vector<Square>{s::b1, s::b2, s::a2});
  generate_and_sort_influence(board_i, s::h4, v_i);
  CHECK(v_i == std::vector<Square>{s::h3, s::g3, s::g4, s::h5, s::g5});
  generate_and_sort_influence(board_i, s::e4, v_i);
  CHECK(v_i == std::vector<Square>{s::f3, s::e3, s::d3, s::f4, s::d4, s::f5,
                                   s::e5, s::d5});
  generate_and_sort_influence(board_i, s::b4, v_i);
  CHECK(v_i == std::vector<Square>{s::c3, s::b3, s::a3, s::c4, s::a4, s::c5,
                                   s::b5, s::a5});
  generate_and_sort_influence(board_i, s::g5, v_i);
  CHECK(v_i == std::vector<Square>{s::h4, s::g4, s::f4, s::h5, s::f5, s::h6,
                                   s::g6, s::f6});
  generate_and_sort_influence(board_i, s::a5, v_i);
  CHECK(v_i == std::vector<Square>{s::b4, s::a4, s::b5, s::b6, s::a6});
  generate_and_sort_influence(board_i, s::h8, v_i);
  CHECK(v_i == std::vector<Square>{s::h7, s::g7, s::g8});
  generate_and_sort_influence(board_i, s::d8, v_i);
  CHECK(v_i == std::vector<Square>{s::e7, s::d7, s::c7, s::e8, s::c8});
  generate_and_sort_influence(board_i, s::a8, v_i);
  CHECK(v_i == std::vector<Square>{s::b7, s::a7, s::b8});
}

TEST_CASE("pawn attack map") {
  board_i.import_fen("8/p2p3p/8/8/8/8/P3P2P/8 w - - 0 1");
  generate_and_sort_influence(board_i, s::h2, v_i);
  CHECK(v_i == std::vector<Square>{s::g3});
  generate_and_sort_influence(board_i, s::e2, v_i);
  CHECK(v_i == std::vector<Square>{s::f3, s::d3});
  generate_and_sort_influence(board_i, s::a2, v_i);
  CHECK(v_i == std::vector<Square>{s::b3});
  generate_and_sort_influence(board_i, s::h7, v_i);
  CHECK(v_i == std::vector<Square>{s::g6});
  generate_and_sort_influence(board_i, s::d7, v_i);
  CHECK(v_i == std::vector<Square>{s::e6, s::c6});
  generate_and_sort_influence(board_i, s::a7, v_i);
  CHECK(v_i == std::vector<Square>{s::b6});
}
