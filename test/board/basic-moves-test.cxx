#include "../../include/Board.h"
#include <catch2/catch_all.hpp>
#include <iostream>

void generate_and_sort_bmt(Board &board, const Square &sq,
                           std::vector<Square> &v) {
  board.update_move_maps();
  Color c = board.what_color(sq);
  if (c == Color::white) {
    v = board.maps->white_moves[sq];
  }
  if (c == Color::black) {
    v = board.maps->black_moves[sq];
  }
  std::sort(v.begin(), v.end());
}

void generate_and_sort_white_king(Board &board, const Square &sq,
                                  std::vector<Square> &v) {
  board.update_move_maps();
  v = board.maps->white_moves[sq];
  std::sort(v.begin(), v.end());
}

void generate_and_sort_black_king(Board &board, const Square &sq,
                                  std::vector<Square> &v) {
  board.update_move_maps();
  v = board.maps->black_moves[sq];
  std::sort(v.begin(), v.end());
}

using s = Square;

Board board_bmt;
std::vector<Square> v_bmt;

TEST_CASE("legal moves pawn") {
  board_bmt.import_fen("8/1p1P2p1/P1P2p2/4p3/3P4/2P2p1p/1P2p1P1/8 w - - 0 1");
  generate_and_sort_bmt(board_bmt, s::g2, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::h3, s::g3, s::f3, s::g4});
  generate_and_sort_bmt(board_bmt, s::e2, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::e1});
  generate_and_sort_bmt(board_bmt, s::b2, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::b3, s::b4});
  generate_and_sort_bmt(board_bmt, s::h3, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::h2, s::g2});
  generate_and_sort_bmt(board_bmt, s::f3, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::g2, s::f2});
  generate_and_sort_bmt(board_bmt, s::f3, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::g2, s::f2});
  generate_and_sort_bmt(board_bmt, s::c3, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::c4});
  generate_and_sort_bmt(board_bmt, s::d4, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::e5, s::d5});
  generate_and_sort_bmt(board_bmt, s::e5, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::e4, s::d4});
  generate_and_sort_bmt(board_bmt, s::f6, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::f5});
  generate_and_sort_bmt(board_bmt, s::c6, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::c7, s::b7});
  generate_and_sort_bmt(board_bmt, s::a6, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::b7, s::a7});
  generate_and_sort_bmt(board_bmt, s::g7, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::g5, s::g6});
  generate_and_sort_bmt(board_bmt, s::d7, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::d8});
  generate_and_sort_bmt(board_bmt, s::b7, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::b5, s::c6, s::b6, s::a6});
}

TEST_CASE("legal moves rook") {
  board_bmt.import_fen("R2R3r/KR6/2R5/R2Rr3/3Rr2r/5r2/6rk/R3r2r w - - 0 1");
  generate_and_sort_bmt(board_bmt, s::h1, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::g1, s::f1});
  generate_and_sort_bmt(board_bmt, s::e1, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::g1, s::f1, s::d1, s::c1, s::b1, s::a1,
                                     s::e2, s::e3});
  generate_and_sort_bmt(board_bmt, s::a1, v_bmt);
  CHECK(v_bmt ==
        std::vector<Square>{s::e1, s::d1, s::c1, s::b1, s::a2, s::a3, s::a4});
  generate_and_sort_bmt(board_bmt, s::g2, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::g1, s::f2, s::e2, s::d2, s::c2, s::b2,
                                     s::a2, s::g3, s::g4, s::g5, s::g6, s::g7,
                                     s::g8});
  generate_and_sort_bmt(board_bmt, s::f3, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::f1, s::f2, s::h3, s::g3, s::e3, s::d3,
                                     s::c3, s::b3, s::a3, s::f4, s::f5, s::f6,
                                     s::f7, s::f8});
  generate_and_sort_bmt(board_bmt, s::h4, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::h3, s::g4, s::f4, s::h5, s::h6, s::h7});
  generate_and_sort_bmt(board_bmt, s::e4, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::e2, s::e3, s::g4, s::f4, s::d4});
  generate_and_sort_bmt(board_bmt, s::d4, v_bmt);
  CHECK(v_bmt ==
        std::vector<Square>{s::d1, s::d2, s::d3, s::e4, s::c4, s::b4, s::a4});
  generate_and_sort_bmt(board_bmt, s::e5, v_bmt);
  CHECK(v_bmt ==
        std::vector<Square>{s::h5, s::g5, s::f5, s::d5, s::e6, s::e7, s::e8});
  generate_and_sort_bmt(board_bmt, s::d5, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::e5, s::c5, s::b5, s::d6, s::d7});
  generate_and_sort_bmt(board_bmt, s::a5, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::a2, s::a3, s::a4, s::c5, s::b5, s::a6});
  generate_and_sort_bmt(board_bmt, s::c6, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::c1, s::c2, s::c3, s::c4, s::c5, s::h6,
                                     s::g6, s::f6, s::e6, s::d6, s::b6, s::a6,
                                     s::c7, s::c8});
  generate_and_sort_bmt(board_bmt, s::b7, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::b1, s::b2, s::b3, s::b4, s::b5, s::b6,
                                     s::h7, s::g7, s::f7, s::e7, s::d7, s::c7,
                                     s::b8});
  generate_and_sort_bmt(board_bmt, s::h8, v_bmt);
  CHECK(v_bmt ==
        std::vector<Square>{s::h5, s::h6, s::h7, s::g8, s::f8, s::e8, s::d8});
  generate_and_sort_bmt(board_bmt, s::d8, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::d6, s::d7, s::h8, s::g8, s::f8, s::e8,
                                     s::c8, s::b8});
  generate_and_sort_bmt(board_bmt, s::a8, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::c8, s::b8});
}

TEST_CASE("legal moves bishop") {
  board_bmt.import_fen("b1kb3B/3b4/3b4/3bB2B/b2bB3/4B3/4B3/b3B1KB w - - 0 1");
  generate_and_sort_bmt(board_bmt, s::h1, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::g2, s::f3});
  generate_and_sort_bmt(board_bmt, s::e1, v_bmt);
  CHECK(v_bmt ==
        std::vector<Square>{s::f2, s::d2, s::g3, s::c3, s::h4, s::b4, s::a5});
  generate_and_sort_bmt(board_bmt, s::a1, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::b2, s::c3});
  generate_and_sort_bmt(board_bmt, s::e2, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::f1, s::d1, s::f3, s::d3, s::g4, s::c4,
                                     s::b5, s::a6});
  generate_and_sort_bmt(board_bmt, s::e3, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::f2, s::d4});
  generate_and_sort_bmt(board_bmt, s::e4, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::b1, s::g2, s::c2, s::f3, s::d3, s::f5,
                                     s::d5, s::g6, s::h7});
  generate_and_sort_bmt(board_bmt, s::d4, v_bmt);
  CHECK(v_bmt ==
        std::vector<Square>{s::b2, s::e3, s::c3, s::e5, s::c5, s::b6, s::a7});
  generate_and_sort_bmt(board_bmt, s::a4, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::d1, s::c2, s::b3, s::b5, s::c6});
  generate_and_sort_bmt(board_bmt, s::h5, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::f3, s::g4, s::g6, s::f7, s::e8});
  generate_and_sort_bmt(board_bmt, s::e5, v_bmt);
  CHECK(v_bmt ==
        std::vector<Square>{s::h2, s::g3, s::f4, s::d4, s::f6, s::d6, s::g7});
  generate_and_sort_bmt(board_bmt, s::d5, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::a2, s::b3, s::e4, s::c4, s::e6, s::c6,
                                     s::f7, s::b7, s::g8});
  generate_and_sort_bmt(board_bmt, s::d6, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::a3, s::b4, s::e5, s::c5, s::e7, s::c7,
                                     s::f8, s::b8});
  generate_and_sort_bmt(board_bmt, s::d7, v_bmt);
  CHECK(v_bmt ==
        std::vector<Square>{s::h3, s::g4, s::f5, s::b5, s::e6, s::c6, s::e8});
  generate_and_sort_bmt(board_bmt, s::h8, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::f6, s::g7});
  generate_and_sort_bmt(board_bmt, s::d8, v_bmt);
  CHECK(v_bmt ==
        std::vector<Square>{s::h4, s::g5, s::a5, s::f6, s::b6, s::e7, s::c7});
  generate_and_sort_bmt(board_bmt, s::a8, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::c6, s::b7});
}

TEST_CASE("legal moves queen") {
  board_bmt.import_fen("q2q3Q/1k6/8/q2q4/4Q2Q/8/6K1/q3Q2Q w - - 0 1");
  generate_and_sort_bmt(board_bmt, s::h1, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::g1, s::f1, s::h2, s::h3});
  generate_and_sort_bmt(board_bmt, s::e1, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::g1, s::f1, s::d1, s::c1, s::b1, s::a1,
                                     s::f2, s::e2, s::d2, s::g3, s::e3, s::c3,
                                     s::b4, s::a5});
  generate_and_sort_bmt(board_bmt, s::a1, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::e1, s::d1, s::c1, s::b1, s::b2, s::a2,
                                     s::c3, s::a3, s::d4, s::a4, s::e5, s::f6,
                                     s::g7, s::h8});
  generate_and_sort_bmt(board_bmt, s::h4, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::h2, s::f2, s::h3, s::g3, s::g4, s::f4,
                                     s::h5, s::g5, s::h6, s::f6, s::h7, s::e7,
                                     s::d8});
  generate_and_sort_bmt(board_bmt, s::e4, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::f3, s::d5});
  generate_and_sort_bmt(board_bmt, s::d5, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::e4, s::c6});
  generate_and_sort_bmt(board_bmt, s::a5, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::e1, s::d2, s::a2, s::c3, s::a3, s::b4,
                                     s::a4, s::c5, s::b5, s::b6, s::a6, s::c7,
                                     s::a7});
  generate_and_sort_bmt(board_bmt, s::h8, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::a1, s::b2, s::c3, s::d4, s::h5, s::e5,
                                     s::h6, s::f6, s::h7, s::g7, s::g8, s::f8,
                                     s::e8, s::d8});
  generate_and_sort_bmt(board_bmt, s::d8, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::h4, s::g5, s::f6, s::d6, s::b6, s::e7,
                                     s::d7, s::c7, s::h8, s::g8, s::f8, s::e8,
                                     s::c8, s::b8});
  generate_and_sort_bmt(board_bmt, s::a8, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::a6, s::a7, s::c8, s::b8});
}

TEST_CASE("legal moves knight") {
  board_bmt.import_fen(
      "NN1N2NN/NN2N1NN/8/N3N1N1/1N5N/8/NN1N2NN/NN2N1NN w - - 0 1");
  // center 4x4
  generate_and_sort_bmt(board_bmt, s::e5, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::f3, s::d3, s::g4, s::c4, s::g6, s::c6,
                                     s::f7, s::d7});
  // b file - 4 squares
  generate_and_sort_bmt(board_bmt, s::b4, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::c2, s::d3, s::d5, s::c6, s::a6});
  // g files - 4 squares
  generate_and_sort_bmt(board_bmt, s::g5, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::h3, s::f3, s::e4, s::e6, s::f7});
  // 2nd rank - 4 squares
  generate_and_sort_bmt(board_bmt, s::d2, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::f1, s::f3, s::b3, s::e4, s::c4});
  // 7th rank - 4 squares
  generate_and_sort_bmt(board_bmt, s::e7, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::f5, s::d5, s::g6, s::c6, s::c8});
  // a file - 4 squares
  generate_and_sort_bmt(board_bmt, s::a5, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::b3, s::c4, s::c6});
  // h file - 4 squares
  generate_and_sort_bmt(board_bmt, s::h4, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::f3, s::f5, s::g6});
  // 1st rank - 4 squares
  generate_and_sort_bmt(board_bmt, s::e1, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::c2, s::f3, s::d3});
  // 8th rank - 4 squares
  generate_and_sort_bmt(board_bmt, s::d8, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::e6, s::c6, s::f7});
  // corners
  generate_and_sort_bmt(board_bmt, s::h1, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::f2, s::g3});
  generate_and_sort_bmt(board_bmt, s::a1, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::c2, s::b3});
  generate_and_sort_bmt(board_bmt, s::h8, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::g6, s::f7});
  generate_and_sort_bmt(board_bmt, s::a8, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::b6, s::c7});
  // inside 2's and 7's
  generate_and_sort_bmt(board_bmt, s::g2, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::e3, s::f4});
  generate_and_sort_bmt(board_bmt, s::b2, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::d1, s::d3, s::c4, s::a4});
  generate_and_sort_bmt(board_bmt, s::g7, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::h5, s::f5, s::e6, s::e8});
  generate_and_sort_bmt(board_bmt, s::b7, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::c5, s::d6});
  // edge 1's and 8's
  generate_and_sort_bmt(board_bmt, s::g1, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::e2, s::h3, s::f3});
  generate_and_sort_bmt(board_bmt, s::b1, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::c3, s::a3});
  generate_and_sort_bmt(board_bmt, s::g8, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::h6, s::f6});
  generate_and_sort_bmt(board_bmt, s::b8, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::c6, s::a6, s::d7});
  // edge 2's and 7's
  generate_and_sort_bmt(board_bmt, s::h2, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::f1, s::f3, s::g4});
  generate_and_sort_bmt(board_bmt, s::a2, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::c1, s::c3});
  generate_and_sort_bmt(board_bmt, s::h7, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::f6, s::f8});
  generate_and_sort_bmt(board_bmt, s::a7, v_bmt);
  CHECK(v_bmt == std::vector<Square>{s::b5, s::c6, s::c8});
}

TEST_CASE("legal moves king") {
  SECTION("white") {
    board_bmt.import_fen(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    generate_and_sort_white_king(board_bmt, s::e1, v_bmt);
    CHECK(v_bmt == std::vector<Square>{});
    board_bmt.import_fen(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1");
    generate_and_sort_white_king(board_bmt, s::e1, v_bmt);
    CHECK(v_bmt == std::vector<Square>{});
    board_bmt.import_fen(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK2R w KQkq - 0 1");
    generate_and_sort_white_king(board_bmt, s::e1, v_bmt);
    CHECK(v_bmt == std::vector<Square>{s::g1, s::f1});
    board_bmt.import_fen(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK1NR w KQkq - 0 1");
    generate_and_sort_white_king(board_bmt, s::e1, v_bmt);
    CHECK(v_bmt == std::vector<Square>{s::f1});
    board_bmt.import_fen(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RN1QKBNR w KQkq - 0 1");
    generate_and_sort_white_king(board_bmt, s::e1, v_bmt);
    CHECK(v_bmt == std::vector<Square>{});
    board_bmt.import_fen(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RN2KBNR w KQkq - 0 1");
    generate_and_sort_white_king(board_bmt, s::e1, v_bmt);
    CHECK(v_bmt == std::vector<Square>{s::d1});
    board_bmt.import_fen(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1");
    generate_and_sort_white_king(board_bmt, s::e1, v_bmt);
    CHECK(v_bmt == std::vector<Square>{s::d1});
    board_bmt.import_fen(
        "rnb1kbnr/ppppqppp/8/8/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1");
    generate_and_sort_white_king(board_bmt, s::e1, v_bmt);
    CHECK(v_bmt == std::vector<Square>{});
    board_bmt.import_fen(
        "rnb1kbnr/pppp1ppp/5q2/8/8/8/PPP4P/RNB1K2R w KQkq - 0 1");
    generate_and_sort_white_king(board_bmt, s::e1, v_bmt);
    CHECK(v_bmt == std::vector<Square>{s::d1, s::e2, s::d2});
    board_bmt.import_fen(
        "rnb1kbnr/pppp1ppp/6q1/8/8/8/PPP4P/RNB1K2R w KQkq - 0 1");
    generate_and_sort_white_king(board_bmt, s::e1, v_bmt);
    CHECK(v_bmt == std::vector<Square>{s::f1, s::d1, s::f2, s::e2, s::d2});
    board_bmt.import_fen(
        "rnb1kbnr/pppp1ppp/6q1/8/8/8/PPP4P/RNB1K2R w KQkq - 0 1");
    generate_and_sort_white_king(board_bmt, s::e1, v_bmt);
    CHECK(v_bmt == std::vector<Square>{s::f1, s::d1, s::f2, s::e2, s::d2});
    board_bmt.import_fen(
        "rnb1kbnr/pppp1ppp/8/8/8/8/PPP4P/RNB1K2R w KQkq - 0 1");
    generate_and_sort_white_king(board_bmt, s::e1, v_bmt);
    CHECK(v_bmt ==
          std::vector<Square>{s::g1, s::f1, s::d1, s::f2, s::e2, s::d2});
    board_bmt.import_fen("8/3p4/1p5r/5p1p/7K/8/8/8 w - - 0 1");
    generate_and_sort_white_king(board_bmt, s::h4, v_bmt);
    CHECK(v_bmt == std::vector<Square>{s::h3, s::g3, s::g5});
    board_bmt.import_fen("8/3p4/1p5r/5p1p/5K2/8/8/8 w - - 0 1");
    generate_and_sort_white_king(board_bmt, s::f4, v_bmt);
    CHECK(v_bmt ==
          std::vector<Square>{s::g3, s::f3, s::e3, s::g5, s::f5, s::e5});
    board_bmt.import_fen("8/3p4/1p5r/5p1p/3K4/8/8/8 w - - 0 1");
    generate_and_sort_white_king(board_bmt, s::d4, v_bmt);
    CHECK(v_bmt ==
          std::vector<Square>{s::e3, s::d3, s::c3, s::c4, s::e5, s::d5});
    board_bmt.import_fen("8/3p4/1p5r/5p1p/1K6/8/8/8 w - - 0 1");
    generate_and_sort_white_king(board_bmt, s::b4, v_bmt);
    CHECK(v_bmt ==
          std::vector<Square>{s::c3, s::b3, s::a3, s::c4, s::a4, s::b5});
  }
  SECTION("black") {
    board_bmt.import_fen(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    generate_and_sort_black_king(board_bmt, s::e8, v_bmt);
    CHECK(v_bmt == std::vector<Square>{});
    board_bmt.import_fen(
        "rnbqkb1r/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    generate_and_sort_black_king(board_bmt, s::e8, v_bmt);
    CHECK(v_bmt == std::vector<Square>{});
    board_bmt.import_fen(
        "rnbqk2r/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    generate_and_sort_black_king(board_bmt, s::e8, v_bmt);
    CHECK(v_bmt == std::vector<Square>{s::g8, s::f8});
    board_bmt.import_fen(
        "rnbqk1nr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    generate_and_sort_black_king(board_bmt, s::e8, v_bmt);
    CHECK(v_bmt == std::vector<Square>{s::f8});
    board_bmt.import_fen(
        "rn1qkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    generate_and_sort_black_king(board_bmt, s::e8, v_bmt);
    CHECK(v_bmt == std::vector<Square>{});
    board_bmt.import_fen(
        "rn2kbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    generate_and_sort_black_king(board_bmt, s::e8, v_bmt);
    CHECK(v_bmt == std::vector<Square>{s::d8});
    board_bmt.import_fen(
        "rnb1kbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    generate_and_sort_black_king(board_bmt, s::e8, v_bmt);
    CHECK(v_bmt == std::vector<Square>{s::d8});
    board_bmt.import_fen(
        "rnbqkbnr/pppp1ppp/8/8/8/8/PPPPQPPP/RNB1KBNR w KQkq - 0 1");
    generate_and_sort_black_king(board_bmt, s::e8, v_bmt);
    CHECK(v_bmt == std::vector<Square>{});
    board_bmt.import_fen(
        "rnb1k2r/ppp4p/8/8/8/5Q2/PPPP1PPP/RNB1KBNR w KQkq - 0 1");
    generate_and_sort_black_king(board_bmt, s::e8, v_bmt);
    CHECK(v_bmt == std::vector<Square>{s::e7, s::d7, s::d8});
    board_bmt.import_fen(
        "rnb1k2r/ppp4p/8/8/8/6Q1/PPPP1PPP/RNB1KBNR w KQkq - 0 1");
    generate_and_sort_black_king(board_bmt, s::e8, v_bmt);
    CHECK(v_bmt == std::vector<Square>{s::f7, s::e7, s::d7, s::f8, s::d8});
    board_bmt.import_fen(
        "rnb1k2r/ppp4p/8/8/8/8/PPPP1PPP/RNB1KBNR w KQkq - 0 1");
    generate_and_sort_black_king(board_bmt, s::e8, v_bmt);
    CHECK(v_bmt ==
          std::vector<Square>{s::f7, s::e7, s::d7, s::g8, s::f8, s::d8});
    board_bmt.import_fen("8/8/8/7k/7P/1P3P1R/3P4/8 w - - 0 1");
    generate_and_sort_black_king(board_bmt, s::h5, v_bmt);
    CHECK(v_bmt == std::vector<Square>{s::h6, s::g6});
    board_bmt.import_fen("8/8/8/5k2/7P/1P3P1R/3P4/8 w - - 0 1");
    generate_and_sort_black_king(board_bmt, s::f5, v_bmt);
    CHECK(v_bmt == std::vector<Square>{s::f4, s::e5, s::g6, s::f6, s::e6});
    board_bmt.import_fen("8/8/8/3k4/7P/1P3P1R/3P4/8 w - - 0 1");
    generate_and_sort_black_king(board_bmt, s::d5, v_bmt);
    CHECK(v_bmt ==
          std::vector<Square>{s::d4, s::e5, s::c5, s::e6, s::d6, s::c6});
    board_bmt.import_fen("8/8/8/8/1k5P/1P3P1R/3P4/8 w - - 0 1");
    generate_and_sort_black_king(board_bmt, s::b4, v_bmt);
    CHECK(v_bmt == std::vector<Square>{s::b3, s::a3, s::c5, s::b5, s::a5});
  }
}
