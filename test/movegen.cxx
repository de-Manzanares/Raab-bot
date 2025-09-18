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
