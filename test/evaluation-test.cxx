#include <catch2/catch_all.hpp>
#include <iostream>
#include "../src/Evaluator.hpp"
#include "../src/Board.hpp"

Board board;

TEST_CASE("simple evaluation")
{
    // TODO not initializing to startpos? --------------------------
    board.import_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    board.update_move_maps();
    CHECK(material_evaluation(&board) == 0.00);
    CHECK(simple_evaluation(&board) == 0.00);
    // --------------------------

    board.import_fen("8/7p/8/5p2/5k2/1P3P2/1r5P/5KR1 w - - 27 59");
    CHECK(eval(&board) == 1);

    board.import_fen("2kr4/pp2ppbp/2n3p1/4p1B1/8/8/PP3PPP/3R2K1 b - - 0 18");
    board.update_move_maps();
    CHECK(eval(&board) == -5);

    board.import_fen("2k5/pp2ppbp/2n3p1/4p1B1/8/8/PP3PPP/3r2K1 w - - 0 19");
    board.update_move_maps();
    CHECK(eval(&board) == -1000);

    board.import_fen("r4k2/1pp3pp/4Q3/p7/8/2PP1N1P/1Pn2RP1/1K1R4 w - - 1 26");
    board.update_move_maps();
    CHECK(eval(&board) == 14);

    board.import_fen("r4k2/1pp3pp/4Q3/p5N1/8/2PP3P/1Pn2RP1/1K1R4 b - - 2 26");
    board.update_move_maps();
    CHECK(eval(&board) == 1000);
}

TEST_CASE("bruh")
{
    board.import_fen("1Q4k1/8/6K1/8/8/8/8/8 b - - 1 1");
    board.update_move_maps();
    CHECK(eval(&board) == 1000);
}