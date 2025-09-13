#include <catch2/catch_all.hpp>

import Board0x88;

TEST_CASE("pawn") {
  SECTION("white") {
    const Board b0;
    CHECK(is_attacked(b0, b3, white) == true);
    const Board b1("rnbqkbnr/pppppppp/8/8/8/8/1P1PPPPP/RNB1KBNR w KQkq - 0 1");
    CHECK(is_attacked(b1, b3, white) == false);
  }
  SECTION("black") {
    const Board b0;
    CHECK(is_attacked(b0, b6, black) == true);
    const Board b1("rnb1kbnr/1p1ppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    CHECK(is_attacked(b1, b6, black) == false);
  }
}
TEST_CASE("knight") {
  SECTION("white") {
    const Board b0("rnbqkbnr/pppppppp/8/8/8/8/P1P1PPPP/RNBQKBNR w KQkq - 0 1");
    CHECK(is_attacked(b0, c3, white) == true);
    const Board b1("rnbqkbnr/pppppppp/8/8/8/8/P1P1PPPP/R1BQKBNR w KQkq - 0 1");
    CHECK(is_attacked(b1, c3, white) == false);
  }
  SECTION("black") {
    const Board b0("rnbqkbnr/p1p1pppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    CHECK(is_attacked(b0, c6, black) == true);
    const Board b1("r1bqkbnr/p1p1pppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    CHECK(is_attacked(b1, c6, black) == false);
  }
}
TEST_CASE("bishop") {
  SECTION("white") {
    const Board b0("rnbqkbnr/pppppppp/8/8/8/8/PPP1PPPP/RNBQKBNR w KQkq - 0 1");
    CHECK(is_attacked(b0, f4, white) == true);
    const Board b1("rnbqkbnr/pppppppp/8/8/8/8/PPP1PPPP/RN1QKBNR w KQkq - 0 1");
    CHECK(is_attacked(b1, f4, white) == false);
  }
  SECTION("black") {
    const Board b0("rnbqkbnr/ppp1pppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    CHECK(is_attacked(b0, f5, black) == true);
    const Board b1("rn1qkbnr/ppp1pppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    CHECK(is_attacked(b1, f5, black) == false);
  }
}
TEST_CASE("rook") {
  SECTION("white") {
    const Board b0("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPP1/RNBQKBNR w KQkq - 0 1");
    CHECK(is_attacked(b0, h5, white) == true);
    const Board b1;
    CHECK(is_attacked(b1, h5, white) == false);
  }
  SECTION("black") {
    const Board b0("rnbqkbnr/ppppppp1/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    CHECK(is_attacked(b0, h4, black) == true);
    const Board b1;
    CHECK(is_attacked(b1, h4, black) == false);
  }
}
TEST_CASE("queen") {
  const Board b0("rnbqkbnr/ppp1pppp/8/8/8/8/PPP1PPPP/RNBQKBNR w KQkq - 0 1");
  const Board b1;
  SECTION("white") {
    CHECK(is_attacked(b0, d4, white) == true);
    CHECK(is_attacked(b1, d4, white) == false);
  }
  SECTION("black") {
    CHECK(is_attacked(b0, d4, black) == true);
    CHECK(is_attacked(b1, d4, black) == false);
  }
}
TEST_CASE("king") {
  const Board b0(
      "rnbqkbnr/pppppppp/8/8/3K1k2/8/PPPPPPPP/RNBQKBNR w HAha - 0 1");
  const Board b1;
  SECTION("white") {
    CHECK(is_attacked(b0, e4, white) == true);
    CHECK(is_attacked(b1, e4, white) == false);
  }
  SECTION("black") {
    CHECK(is_attacked(b0, e4, black) == true);
    CHECK(is_attacked(b1, e4, black) == false);
  }
}
