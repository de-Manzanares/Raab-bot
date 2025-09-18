#include <algorithm>
#include <catch2/catch_all.hpp>
#include <ranges>

import Board0x88;

constexpr bool has(std::array<Move, 256> ml, const Move m) {
  return std::ranges::find(ml, m) != ml.end();
}

constexpr bool is_immobile(const std::array<Move, 256> &ml, Square sq) {
  return std::none_of(ml.begin(), ml.end(),
                      [sq](const Move &m) { return m.from == sq; });
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

TEST_CASE("king") {
  SECTION("normal") {
    SECTION("white") {
      Board b0("8/8/8/2k2K2/8/8/8/8 w - - 0 1");
      auto ml = movegen(b0);
      CHECK(has(ml, Move{f5, e4, normal}));
      CHECK(has(ml, Move{f5, f4, normal}));
      CHECK(has(ml, Move{f5, g4, normal}));
      CHECK(has(ml, Move{f5, e5, normal}));
      CHECK(has(ml, Move{f5, g5, normal}));
      CHECK(has(ml, Move{f5, e6, normal}));
      CHECK(has(ml, Move{f5, f6, normal}));
      CHECK(has(ml, Move{f5, g6, normal}));
    }
    SECTION("black") {
      Board b0("8/8/8/2k2K2/8/8/8/8 b - - 0 1");
      auto ml = movegen(b0);
      CHECK(has(ml, Move{c5, b4, normal}));
      CHECK(has(ml, Move{c5, c4, normal}));
      CHECK(has(ml, Move{c5, d4, normal}));
      CHECK(has(ml, Move{c5, b5, normal}));
      CHECK(has(ml, Move{c5, d5, normal}));
      CHECK(has(ml, Move{c5, b6, normal}));
      CHECK(has(ml, Move{c5, c6, normal}));
      CHECK(has(ml, Move{c5, d6, normal}));
    }
  }
  SECTION("capture") {
    SECTION("white") {
      Board b0("8/8/1QRBqrb1/1NkPnKp1/1PPPppp1/8/8/8 w - - 0 1");
      auto ml = movegen(b0);
      CHECK(has(ml, Move{f5, e4, capture, .c_piece = pawn}));
      CHECK(has(ml, Move{f5, f4, capture, .c_piece = pawn}));
      CHECK(has(ml, Move{f5, g4, capture, .c_piece = pawn}));
      CHECK(has(ml, Move{f5, e5, capture, .c_piece = knight}));
      CHECK(has(ml, Move{f5, g5, capture, .c_piece = pawn}));
      CHECK(has(ml, Move{f5, e6, capture, .c_piece = queen}));
      CHECK(has(ml, Move{f5, f6, capture, .c_piece = rook}));
      CHECK(has(ml, Move{f5, g6, capture, .c_piece = bishop}));
    }
    SECTION("black") {
      Board b0("8/8/1QRBqrb1/1NkPnKp1/1PPPppp1/8/8/8 b - - 0 1");
      auto ml = movegen(b0);
      CHECK(has(ml, Move{c5, b4, capture, .c_piece = pawn}));
      CHECK(has(ml, Move{c5, c4, capture, .c_piece = pawn}));
      CHECK(has(ml, Move{c5, d4, capture, .c_piece = pawn}));
      CHECK(has(ml, Move{c5, b5, capture, .c_piece = knight}));
      CHECK(has(ml, Move{c5, d5, capture, .c_piece = pawn}));
      CHECK(has(ml, Move{c5, b6, capture, .c_piece = queen}));
      CHECK(has(ml, Move{c5, c6, capture, .c_piece = rook}));
      CHECK(has(ml, Move{c5, d6, capture, .c_piece = bishop}));
    }
  }
  SECTION("obstructed") {
    SECTION("white") {
      Board b0("8/8/1qrbQRB1/1nkpNKP1/1pppPPP1/8/8/8 w - - 0 1");
      auto ml = movegen(b0);
      CHECK(is_immobile(ml, f5));
    }
    SECTION("black") {
      Board b0("8/8/1qrbQRB1/1nkpNKP1/1pppPPP1/8/8/8 b - - 0 1");
      auto ml = movegen(b0);
      CHECK(is_immobile(ml, c5));
    }
  }
}

TEST_CASE("knight") {
  SECTION("normal") {
    SECTION("white") {
      Board b0("8/8/2n5/8/8/5N2/8/8 w - - 0 1");
      auto ml = movegen(b0);
      CHECK(has(ml, Move{f3, e1, normal}));
      CHECK(has(ml, Move{f3, g1, normal}));
      CHECK(has(ml, Move{f3, d2, normal}));
      CHECK(has(ml, Move{f3, h2, normal}));
      CHECK(has(ml, Move{f3, d4, normal}));
      CHECK(has(ml, Move{f3, h4, normal}));
      CHECK(has(ml, Move{f3, e5, normal}));
      CHECK(has(ml, Move{f3, g5, normal}));
    }
    SECTION("black") {
      Board b0("8/8/2n5/8/8/5N2/8/8 b - - 0 1");
      auto ml = movegen(b0);
      CHECK(has(ml, Move{c6, b4, normal}));
      CHECK(has(ml, Move{c6, d4, normal}));
      CHECK(has(ml, Move{c6, a5, normal}));
      CHECK(has(ml, Move{c6, e5, normal}));
      CHECK(has(ml, Move{c6, a7, normal}));
      CHECK(has(ml, Move{c6, e7, normal}));
      CHECK(has(ml, Move{c6, b8, normal}));
      CHECK(has(ml, Move{c6, d8, normal}));
    }
  }
  SECTION("capture") {
    SECTION("white") {
      Board b0("1Q1R4/B3N3/2n5/P5b1/1R5q/5N2/3b3n/4q1r1 w - - 0 1");
      auto ml = movegen(b0);
      CHECK(has(ml, Move{f3, e1, capture, .c_piece = queen}));
      CHECK(has(ml, Move{f3, g1, capture, .c_piece = rook}));
      CHECK(has(ml, Move{f3, d2, capture, .c_piece = bishop}));
      CHECK(has(ml, Move{f3, h2, capture, .c_piece = knight}));
      CHECK(has(ml, Move{f3, d4, normal}));
      CHECK(has(ml, Move{f3, h4, capture, .c_piece = queen}));
      CHECK(has(ml, Move{f3, e5, normal}));
      CHECK(has(ml, Move{f3, g5, capture, .c_piece = bishop}));
    }
    SECTION("black") {
      Board b0("1Q1R4/B3N3/2n5/P5b1/1R5q/5N2/3b3n/4q1r1 b - - 0 1");
      auto ml = movegen(b0);
      CHECK(has(ml, Move{c6, b4, capture, .c_piece = rook}));
      CHECK(has(ml, Move{c6, d4, normal}));
      CHECK(has(ml, Move{c6, a5, capture, .c_piece = pawn}));
      CHECK(has(ml, Move{c6, e5, normal}));
      CHECK(has(ml, Move{c6, a7, capture, .c_piece = bishop}));
      CHECK(has(ml, Move{c6, e7, capture, .c_piece = knight}));
      CHECK(has(ml, Move{c6, b8, capture, .c_piece = queen}));
      CHECK(has(ml, Move{c6, d8, capture, .c_piece = rook}));
    }
  }
  SECTION("obstructed") {
    SECTION("white") {
      Board b0("n7/2p5/1p6/8/8/6P1/5P2/7N w - - 0 1");
      auto ml = movegen(b0);
      CHECK(is_immobile(ml, h1));
    }
    SECTION("black") {
      Board b0("n7/2p5/1p6/8/8/6P1/5P2/7N b - - 0 1");
      auto ml = movegen(b0);
      CHECK(is_immobile(ml, h1));
    }
  }
}
