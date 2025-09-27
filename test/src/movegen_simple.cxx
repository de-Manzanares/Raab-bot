#include <algorithm>
#include <catch2/catch_all.hpp>
#include <concepts>
#include <ranges>

import all_modules;

template <class Container>
  requires std::default_initializable<typename Container::value_type>
void fill_default_init(Container &c) {
  using T = Container::value_type;
  std::ranges::fill(c, T{});
}

template <class Container>
  requires std::is_same_v<typename Container::value_type, Move>
constexpr bool has(Container ml, const Move m) {
  return std::ranges::find(ml, m) != ml.end();
}

template <class Container>
  requires std::is_same_v<typename Container::value_type, Move>
constexpr bool is_immobile(const Container &c, Square sq) {
  return std::ranges::none_of(c,
                              [sq](const Move &m) { return m.from_sq == sq; });
}

template <class Container>
  requires std::is_same_v<typename Container::value_type, Move>
constexpr sz_t has_n_moves(const Container &ml, Square sq, Flag f) {
  sz_t n_moves = 0;
  std::ranges::for_each(ml, [&n_moves, sq, f](const Move &m) {
    if (m.from_sq == sq && m.flag == f) {
      n_moves++;
    }
  });
  return n_moves;
}

std::array<Move, 124> ml{};

TEST_CASE("castling") {
  SECTION("can castle") {
    SECTION("white") {
      fill_default_init(ml);
      Board b0("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1");
      movegen(b0, ml.begin());
      CHECK(has(ml, Move{.from_sq = e1, .to_sq = g1, .flag = castle}));
      CHECK(has(ml, Move{.from_sq = e1, .to_sq = c1, .flag = castle}));
    }
    SECTION("black") {
      fill_default_init(ml);
      Board b0("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R b KQkq - 0 1");
      movegen(b0, ml.begin());
      CHECK(has(ml, Move{e8, g8, .flag = castle}));
      CHECK(has(ml, Move{e8, c8, .flag = castle}));
    }
  }
  SECTION("cannot castle") {
    SECTION("squares not empty") {
      SECTION("white") {
        fill_default_init(ml);
        Board b0;
        movegen(b0, ml.begin());
        CHECK(!has(ml, Move{e1, g1, .flag = castle}));
        CHECK(!has(ml, Move{e1, c1, .flag = castle}));
      }
      SECTION("black") {
        fill_default_init(ml);
        Board b0("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
        movegen(b0, ml.begin());
        CHECK(!has(ml, Move{e8, g8, .flag = castle}));
        CHECK(!has(ml, Move{e8, c8, .flag = castle}));
      }
    }
    SECTION("squares attacked") {
      SECTION("white") {
        fill_default_init(ml);
        Board b0("r3k2r/8/8/3Q4/3q4/8/8/R3K2R w KQkq - 0 1");
        movegen(b0, ml.begin());
        CHECK(!has(ml, Move{e1, g1, .flag = castle}));
        CHECK(!has(ml, Move{e1, c1, .flag = castle}));
      }
      SECTION("black") {
        fill_default_init(ml);
        Board b0("r3k2r/8/8/3Q4/3q4/8/8/R3K2R w KQkq - 0 1");
        movegen(b0, ml.begin());
        CHECK(!has(ml, Move{e8, g8, .flag = castle}));
        CHECK(!has(ml, Move{e8, c8, .flag = castle}));
      }
    }
    SECTION("no castling rights") {
      SECTION("white") {
        fill_default_init(ml);
        Board b0("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w kq - 0 1");
        movegen(b0, ml.begin());
        CHECK(!has(ml, Move{e1, g1, .flag = castle}));
        CHECK(!has(ml, Move{e1, c1, .flag = castle}));
      }
      SECTION("black") {
        fill_default_init(ml);
        Board b0("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQ - 0 1");
        movegen(b0, ml.begin());
        CHECK(!has(ml, Move{e8, g8, .flag = castle}));
        CHECK(!has(ml, Move{e8, c8, .flag = castle}));
      }
    }
  }
}

TEST_CASE("pawn") {
  SECTION("non-capture non-promotion moves") {
    SECTION("white") {
      fill_default_init(ml);
      Board b0("8/p7/1p6/8/8/1P6/P7/8 w - - 0 1");
      movegen(b0, ml.begin());
      CHECK(has(ml, Move{a2, a3, .flag = normal}));
      CHECK(has(ml, Move{a2, a4, .flag = double_push}));
      CHECK(has(ml, Move{b3, b4, .flag = normal}));
    }
    SECTION("black") {
      fill_default_init(ml);
      Board b0("8/p7/1p6/8/8/1P6/P7/8 b - - 0 1");
      movegen(b0, ml.begin());
      CHECK(has(ml, Move{a7, a6, .flag = normal}));
      CHECK(has(ml, Move{a7, a5, .flag = double_push}));
      CHECK(has(ml, Move{b6, b5, .flag = normal}));
    }
  }
  SECTION("non-capture promotion moves") {
    SECTION("white") {
      fill_default_init(ml);
      Board b0("8/3P4/8/8/8/8/8/8 w - - 0 1");
      movegen(b0, ml.begin());
      CHECK(has(ml, Move{d7, d8, .flag = promotion, .prom_p = queen}));
      CHECK(has(ml, Move{d7, d8, .flag = promotion, .prom_p = rook}));
      CHECK(has(ml, Move{d7, d8, .flag = promotion, .prom_p = bishop}));
      CHECK(has(ml, Move{d7, d8, .flag = promotion, .prom_p = knight}));
    }
    SECTION("black") {
      fill_default_init(ml);
      Board b0("8/8/8/8/8/8/3p4/8 b - - 0 1");
      movegen(b0, ml.begin());
      CHECK(has(ml, Move{d2, d1, .flag = promotion, .prom_p = queen}));
      CHECK(has(ml, Move{d2, d1, .flag = promotion, .prom_p = rook}));
      CHECK(has(ml, Move{d2, d1, .flag = promotion, .prom_p = bishop}));
      CHECK(has(ml, Move{d2, d1, .flag = promotion, .prom_p = knight}));
    }
  }
  SECTION("non-promotion catpure moves") {
    SECTION("white") {
      fill_default_init(ml);
      Board b0("8/8/8/1q3q1q/P1P3P1/8/8/8 w - - 0 1");
      movegen(b0, ml.begin());
      CHECK(has(ml, Move{a4, b5, .flag = capture, .cap_piece = queen}));
      CHECK(has(ml, Move{c4, b5, .flag = capture, .cap_piece = queen}));
      CHECK(has(ml, Move{g4, f5, .flag = capture, .cap_piece = queen}));
      CHECK(has(ml, Move{g4, h5, .flag = capture, .cap_piece = queen}));
    }
    SECTION("black") {
      fill_default_init(ml);
      Board b0("8/8/p1p3p1/1Q3Q1Q/8/8/8/8 b - - 0 1");
      movegen(b0, ml.begin());
      CHECK(has(ml, Move{a6, b5, .flag = capture, .cap_piece = queen}));
      CHECK(has(ml, Move{c6, b5, .flag = capture, .cap_piece = queen}));
      CHECK(has(ml, Move{g6, f5, .flag = capture, .cap_piece = queen}));
      CHECK(has(ml, Move{g6, h5, .flag = capture, .cap_piece = queen}));
    }
  }
  SECTION("promotion capture moves") {
    SECTION("white") {
      fill_default_init(ml);
      Board b0("1q1q4/2P5/8/8/8/8/8/8 w - - 0 1");
      movegen(b0, ml.begin());
      CHECK(has(ml, Move{c7, b8, .flag = prom_capture, .cap_piece = queen,
                         .prom_p = queen}));
      CHECK(has(ml, Move{c7, d8, .flag = prom_capture, .cap_piece = queen,
                         .prom_p = queen}));
    }
  }
}

TEST_CASE("king") {
  SECTION("normal") {
    SECTION("white") {
      fill_default_init(ml);
      Board b0("8/8/8/2k2K2/8/8/8/8 w - - 0 1");
      movegen(b0, ml.begin());
      CHECK(has(ml, Move{f5, e4, .flag = normal}));
      CHECK(has(ml, Move{f5, f4, .flag = normal}));
      CHECK(has(ml, Move{f5, g4, .flag = normal}));
      CHECK(has(ml, Move{f5, e5, .flag = normal}));
      CHECK(has(ml, Move{f5, g5, .flag = normal}));
      CHECK(has(ml, Move{f5, e6, .flag = normal}));
      CHECK(has(ml, Move{f5, f6, .flag = normal}));
      CHECK(has(ml, Move{f5, g6, .flag = normal}));
    }
    SECTION("black") {
      fill_default_init(ml);
      Board b0("8/8/8/2k2K2/8/8/8/8 b - - 0 1");
      movegen(b0, ml.begin());
      CHECK(has(ml, Move{c5, b4, .flag = normal}));
      CHECK(has(ml, Move{c5, c4, .flag = normal}));
      CHECK(has(ml, Move{c5, d4, .flag = normal}));
      CHECK(has(ml, Move{c5, b5, .flag = normal}));
      CHECK(has(ml, Move{c5, d5, .flag = normal}));
      CHECK(has(ml, Move{c5, b6, .flag = normal}));
      CHECK(has(ml, Move{c5, c6, .flag = normal}));
      CHECK(has(ml, Move{c5, d6, .flag = normal}));
    }
  }
  SECTION("capture") {
    SECTION("white") {
      fill_default_init(ml);
      Board b0("8/8/1QRBqrb1/1NkPnKp1/1PPPppp1/8/8/8 w - - 0 1");
      movegen(b0, ml.begin());
      CHECK(has(ml, Move{f5, e4, .flag = capture, .cap_piece = pawn}));
      CHECK(has(ml, Move{f5, f4, .flag = capture, .cap_piece = pawn}));
      CHECK(has(ml, Move{f5, g4, .flag = capture, .cap_piece = pawn}));
      CHECK(has(ml, Move{f5, e5, .flag = capture, .cap_piece = knight}));
      CHECK(has(ml, Move{f5, g5, .flag = capture, .cap_piece = pawn}));
      CHECK(has(ml, Move{f5, e6, .flag = capture, .cap_piece = queen}));
      CHECK(has(ml, Move{f5, f6, .flag = capture, .cap_piece = rook}));
      CHECK(has(ml, Move{f5, g6, .flag = capture, .cap_piece = bishop}));
    }
    SECTION("black") {
      fill_default_init(ml);
      Board b0("8/8/1QRBqrb1/1NkPnKp1/1PPPppp1/8/8/8 b - - 0 1");
      movegen(b0, ml.begin());
      CHECK(has(ml, Move{c5, b4, .flag = capture, .cap_piece = pawn}));
      CHECK(has(ml, Move{c5, c4, .flag = capture, .cap_piece = pawn}));
      CHECK(has(ml, Move{c5, d4, .flag = capture, .cap_piece = pawn}));
      CHECK(has(ml, Move{c5, b5, .flag = capture, .cap_piece = knight}));
      CHECK(has(ml, Move{c5, d5, .flag = capture, .cap_piece = pawn}));
      CHECK(has(ml, Move{c5, b6, .flag = capture, .cap_piece = queen}));
      CHECK(has(ml, Move{c5, c6, .flag = capture, .cap_piece = rook}));
      CHECK(has(ml, Move{c5, d6, .flag = capture, .cap_piece = bishop}));
    }
  }
  SECTION("obstructed") {
    SECTION("white") {
      fill_default_init(ml);
      Board b0("8/8/1qrbQRB1/1nkpNKP1/1pppPPP1/8/8/8 w - - 0 1");
      movegen(b0, ml.begin());
      CHECK(is_immobile(ml, f5));
    }
    SECTION("black") {
      fill_default_init(ml);
      Board b0("8/8/1qrbQRB1/1nkpNKP1/1pppPPP1/8/8/8 b - - 0 1");
      movegen(b0, ml.begin());
      CHECK(is_immobile(ml, c5));
    }
  }
}

TEST_CASE("knight") {
  SECTION("normal") {
    SECTION("white") {
      fill_default_init(ml);
      Board b0("8/8/2n5/8/8/5N2/8/8 w - - 0 1");
      movegen(b0, ml.begin());
      CHECK(has(ml, Move{f3, e1, .flag = normal}));
      CHECK(has(ml, Move{f3, g1, .flag = normal}));
      CHECK(has(ml, Move{f3, d2, .flag = normal}));
      CHECK(has(ml, Move{f3, h2, .flag = normal}));
      CHECK(has(ml, Move{f3, d4, .flag = normal}));
      CHECK(has(ml, Move{f3, h4, .flag = normal}));
      CHECK(has(ml, Move{f3, e5, .flag = normal}));
      CHECK(has(ml, Move{f3, g5, .flag = normal}));
    }
    SECTION("black") {
      fill_default_init(ml);
      Board b0("8/8/2n5/8/8/5N2/8/8 b - - 0 1");
      movegen(b0, ml.begin());
      CHECK(has(ml, Move{c6, b4, .flag = normal}));
      CHECK(has(ml, Move{c6, d4, .flag = normal}));
      CHECK(has(ml, Move{c6, a5, .flag = normal}));
      CHECK(has(ml, Move{c6, e5, .flag = normal}));
      CHECK(has(ml, Move{c6, a7, .flag = normal}));
      CHECK(has(ml, Move{c6, e7, .flag = normal}));
      CHECK(has(ml, Move{c6, b8, .flag = normal}));
      CHECK(has(ml, Move{c6, d8, .flag = normal}));
    }
  }
  SECTION("capture") {
    SECTION("white") {
      fill_default_init(ml);
      Board b0("1Q1R4/B3N3/2n5/P5b1/1R5q/5N2/3b3n/4q1r1 w - - 0 1");
      movegen(b0, ml.begin());
      CHECK(has(ml, Move{f3, e1, .flag = capture, .cap_piece = queen}));
      CHECK(has(ml, Move{f3, g1, .flag = capture, .cap_piece = rook}));
      CHECK(has(ml, Move{f3, d2, .flag = capture, .cap_piece = bishop}));
      CHECK(has(ml, Move{f3, h2, .flag = capture, .cap_piece = knight}));
      CHECK(has(ml, Move{f3, d4, .flag = normal}));
      CHECK(has(ml, Move{f3, h4, .flag = capture, .cap_piece = queen}));
      CHECK(has(ml, Move{f3, e5, .flag = normal}));
      CHECK(has(ml, Move{f3, g5, .flag = capture, .cap_piece = bishop}));
    }
    SECTION("black") {
      fill_default_init(ml);
      Board b0("1Q1R4/B3N3/2n5/P5b1/1R5q/5N2/3b3n/4q1r1 b - - 0 1");
      movegen(b0, ml.begin());
      CHECK(has(ml, Move{c6, b4, .flag = capture, .cap_piece = rook}));
      CHECK(has(ml, Move{c6, d4, .flag = normal}));
      CHECK(has(ml, Move{c6, a5, .flag = capture, .cap_piece = pawn}));
      CHECK(has(ml, Move{c6, e5, .flag = normal}));
      CHECK(has(ml, Move{c6, a7, .flag = capture, .cap_piece = bishop}));
      CHECK(has(ml, Move{c6, e7, .flag = capture, .cap_piece = knight}));
      CHECK(has(ml, Move{c6, b8, .flag = capture, .cap_piece = queen}));
      CHECK(has(ml, Move{c6, d8, .flag = capture, .cap_piece = rook}));
    }
  }
  SECTION("obstructed") {
    SECTION("white") {
      fill_default_init(ml);
      Board b0("n7/2p5/1p6/8/8/6P1/5P2/7N w - - 0 1");
      movegen(b0, ml.begin());
      CHECK(is_immobile(ml, h1));
    }
    SECTION("black") {
      fill_default_init(ml);
      Board b0("n7/2p5/1p6/8/8/6P1/5P2/7N b - - 0 1");
      movegen(b0, ml.begin());
      CHECK(is_immobile(ml, h1));
    }
  }
}

TEST_CASE("bishop") {
  SECTION("white") {
    fill_default_init(ml);
    Board b0("8/7r/2r3r1/3b4/4B3/8/6r1/8 w - - 0 1");
    movegen(b0, ml.begin());
    CHECK(has_n_moves(ml, e4, normal) == 5);
    CHECK(has_n_moves(ml, e4, capture) == 3);
  }
  SECTION("black") {
    fill_default_init(ml);
    Board b0("6R1/1R3R2/8/3b4/4B3/5R2/8/8 b - - 0 1");
    movegen(b0, ml.begin());
    CHECK(has_n_moves(ml, d5, normal) == 5);
    CHECK(has_n_moves(ml, d5, capture) == 3);
  }
}

TEST_CASE("rook") {
  SECTION("white") {
    fill_default_init(ml);
    Board b0("3r4/4r3/8/3r2RR/rr2R3/8/3R4/4R3 w - - 0 1");
    movegen(b0, ml.begin());
    CHECK(has_n_moves(ml, e4, normal) == 9);
    CHECK(has_n_moves(ml, e4, capture) == 2);
  }
  SECTION("black") {
    fill_default_init(ml);
    Board b0("3r4/4r3/8/3r2RR/rr2R3/8/3R4/4R3 b - - 0 1");
    movegen(b0, ml.begin());
    CHECK(has_n_moves(ml, d5, normal) == 9);
    CHECK(has_n_moves(ml, d5, capture) == 2);
  }
}

TEST_CASE("queen") {
  SECTION("white") {
    fill_default_init(ml);
    Board b0("3q4/4q3/8/3q2QQ/qq2Q3/8/3Q4/4Q3 w - - 0 1");
    movegen(b0, ml.begin());
    CHECK(has_n_moves(ml, e4, normal) == 18);
    CHECK(has_n_moves(ml, e4, capture) == 3);
  }
  SECTION("black") {
    fill_default_init(ml);
    Board b0("3q4/4q3/8/3q2QQ/qq2Q3/8/3Q4/4Q3 b - - 0 1");
    movegen(b0, ml.begin());
    CHECK(has_n_moves(ml, d5, normal) == 18);
    CHECK(has_n_moves(ml, d5, capture) == 3);
  }
}
