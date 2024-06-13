#include <catch2/catch_all.hpp>
#include <iostream>

#include "../../include/Eval.h"
#include "../../include/Node.h"

Board board_et;

TEST_CASE("simple evaluation")
{
    board_et.import_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    board_et.update_move_maps();
    CHECK(Eval::material_evaluation(&board_et) == 0.00);
    CHECK(Eval::simple_evaluation(&board_et) == 0.00);
    // --------------------------

    board_et.import_fen("8/7p/8/5p2/5k2/1P3P2/1r5P/5KR1 w - - 27 59");
    CHECK(Eval::eval(&board_et) == 1);

    board_et.import_fen("2kr4/pp2ppbp/2n3p1/4p1B1/8/8/PP3PPP/3R2K1 b - - 0 18");
    board_et.update_move_maps();
    CHECK(Eval::eval(&board_et) == -5);

    board_et.import_fen("2k5/pp2ppbp/2n3p1/4p1B1/8/8/PP3PPP/3r2K1 w - - 0 19");
    board_et.update_move_maps();
    CHECK(Eval::eval(&board_et) == -1000);

    board_et.import_fen("r4k2/1pp3pp/4Q3/p7/8/2PP1N1P/1Pn2RP1/1K1R4 w - - 1 26");
    board_et.update_move_maps();
    CHECK(Eval::eval(&board_et) == 14);

    board_et.import_fen("r4k2/1pp3pp/4Q3/p5N1/8/2PP3P/1Pn2RP1/1K1R4 b - - 2 26");
    board_et.update_move_maps();
    CHECK(Eval::eval(&board_et) == 1000);
}

TEST_CASE("bruh")
{
    board_et.import_fen("1Q4k1/8/6K1/8/8/8/8/8 b - - 1 1");
    board_et.update_move_maps();
    CHECK(Eval::eval(&board_et) == 1000);
}

TEST_CASE("updated eval")
{
    // Node *n = new Node("7k/3R4/2R5/8/8/8/8/2K5 w - - 0 1");
    // n->spawn(3);
    Node *n0 = new Node;
    n0->spawn_depth_first(2);
}

TEST_CASE("material ratio")
{
    double ratio_multiplier = 2.0;
    Board b;
    b.import_fen("r6N/2r5/5R1N/1k6/6R1/n3K3/8/8 w - - 0 1");
    CHECK(Eval::material_ratio(&b) == ratio_multiplier * 3.0 / 29.0);
    b.import_fen("R6n/2R5/5r1n/1K6/6r1/N3k3/8/8 w - - 0 1");
    CHECK(Eval::material_ratio(&b) == ratio_multiplier * -3.0 / 29.0);
    b.import_fen("8/2r5/5R2/1k6/6R1/4K3/8/8 w - - 0 1");
    CHECK(Eval::material_ratio(&b) == ratio_multiplier * 5.0 / 15.0);
    b.import_fen("8/2R5/5r2/1K6/6r1/4k3/8/8 w - - 0 1");
    CHECK(Eval::material_ratio(&b) == ratio_multiplier * -5.0 / 15.0);
}