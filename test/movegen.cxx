#include <algorithm>
#include <catch2/catch_all.hpp>
#include <ranges>

import chess.types;
import Board;
import movegen;

bool has(std::array<Move, 256> ml, const Move m) {
  return std::ranges::find(ml, m) != ml.end();
}

TEST_CASE("castling") {
  SECTION("can castle") {
    SECTION("white") {
      Board b0("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1");
      auto ml = movegen(b0);
      CHECK(has(ml, Move{e1, g1, castle}));
      CHECK(has(ml, Move{e1, c1, castle}));
    }
    SECTION("black") {
      Board b0("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R b KQkq - 0 1");
      auto ml = movegen(b0);
      CHECK(has(ml, Move{e8, g8, castle}));
      CHECK(has(ml, Move{e8, c8, castle}));
    }
  }
  SECTION("cannot castle") {
    SECTION("squares not empty") {
      SECTION("white") {
        Board b0;
        auto ml = movegen(b0);
        CHECK(!has(ml, Move{e1, g1, castle}));
        CHECK(!has(ml, Move{e1, c1, castle}));
      }
      SECTION("black") {
        Board b0("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
        auto ml = movegen(b0);
        CHECK(!has(ml, Move{e8, g8, castle}));
        CHECK(!has(ml, Move{e8, c8, castle}));
      }
    }
    SECTION("squares attacked") {
      SECTION("white") {
        Board b0("r3k2r/8/8/3Q4/3q4/8/8/R3K2R w KQkq - 0 1");
        auto ml = movegen(b0);
        CHECK(!has(ml, Move{e1, g1, castle}));
        CHECK(!has(ml, Move{e1, c1, castle}));
      }
      SECTION("black") {
        Board b0("r3k2r/8/8/3Q4/3q4/8/8/R3K2R w KQkq - 0 1");
        auto ml = movegen(b0);
        CHECK(!has(ml, Move{e8, g8, castle}));
        CHECK(!has(ml, Move{e8, c8, castle}));
      }
    }
    SECTION("no castling rights") {
      SECTION("white") {
        Board b0("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w kq - 0 1");
        auto ml = movegen(b0);
        CHECK(!has(ml, Move{e1, g1, castle}));
        CHECK(!has(ml, Move{e1, c1, castle}));
      }
      SECTION("black") {
        Board b0("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQ - 0 1");
        auto ml = movegen(b0);
        CHECK(!has(ml, Move{e8, g8, castle}));
        CHECK(!has(ml, Move{e8, c8, castle}));
      }
    }
  }
}

TEST_CASE("pawn") {
  SECTION("non-capture non-promotion moves") {
    SECTION("white") {
      Board b0("8/p7/1p6/8/8/1P6/P7/8 w - - 0 1");
      auto ml = movegen(b0);
      CHECK(has(ml, Move{a2, a3, normal}));
      CHECK(has(ml, Move{a2, a4, en_passant}));
      CHECK(has(ml, Move{b3, b4, normal}));
    }
    SECTION("black") {
      Board b0("8/p7/1p6/8/8/1P6/P7/8 b - - 0 1");
      auto ml = movegen(b0);
      CHECK(has(ml, Move{a7, a6, normal}));
      CHECK(has(ml, Move{a7, a5, en_passant}));
      CHECK(has(ml, Move{b6, b5, normal}));
    }
  }
  SECTION("non-capture promotion moves") {
    SECTION("white") {
      Board b0("8/3P4/8/8/8/8/8/8 w - - 0 1");
      auto ml = movegen(b0);
      CHECK(has(ml, Move{d7, d8, promotion, .p_piece = queen}));
      CHECK(has(ml, Move{d7, d8, promotion, .p_piece = rook}));
      CHECK(has(ml, Move{d7, d8, promotion, .p_piece = bishop}));
      CHECK(has(ml, Move{d7, d8, promotion, .p_piece = knight}));
    }
    SECTION("black") {
      Board b0("8/8/8/8/8/8/3p4/8 b - - 0 1");
      auto ml = movegen(b0);
      CHECK(has(ml, Move{d2, d1, promotion, .p_piece = queen}));
      CHECK(has(ml, Move{d2, d1, promotion, .p_piece = rook}));
      CHECK(has(ml, Move{d2, d1, promotion, .p_piece = bishop}));
      CHECK(has(ml, Move{d2, d1, promotion, .p_piece = knight}));
    }
  }
  SECTION("non-promotion catpure moves") {
    SECTION("white") {
      Board b0("8/8/8/1q3q1q/P1P3P1/8/8/8 w - - 0 1");
      auto ml = movegen(b0);
      CHECK(has(ml, Move{a4, b5, capture, .c_piece = queen}));
      CHECK(has(ml, Move{c4, b5, capture, .c_piece = queen}));
      CHECK(has(ml, Move{g4, f5, capture, .c_piece = queen}));
      CHECK(has(ml, Move{g4, h5, capture, .c_piece = queen}));
    }
    SECTION("black") {
      Board b0("8/8/p1p3p1/1Q3Q1Q/8/8/8/8 b - - 0 1");
      auto ml = movegen(b0);
      CHECK(has(ml, Move{a6, b5, capture, .c_piece = queen}));
      CHECK(has(ml, Move{c6, b5, capture, .c_piece = queen}));
      CHECK(has(ml, Move{g6, f5, capture, .c_piece = queen}));
      CHECK(has(ml, Move{g6, h5, capture, .c_piece = queen}));
    }
  }
  SECTION("promotion capture moves") {
    SECTION("white") {
      Board b0("1q1q4/2P5/8/8/8/8/8/8 w - - 0 1");
      auto ml = movegen(b0);
      CHECK(has(
          ml, Move{c7, b8, prom_capture, .c_piece = queen, .p_piece = queen}));
      CHECK(has(
          ml, Move{c7, d8, prom_capture, .c_piece = queen, .p_piece = queen}));
    }
  }
}