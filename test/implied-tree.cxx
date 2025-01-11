//     ____              __          __          __
//    / __ \____ _____ _/ /_        / /_  ____  / /_
//   / /_/ / __ `/ __ `/ __ \______/ __ \/ __ \/ __
//  / _, _/ /_/ / /_/ / /_/ /_____/ /_/ / /_/ / /_
// /_/ |_|\__,_/\__,_/_.___/     /_.___/\____/\__/
//
// Copyright (c) 2025 de-Manzanares
// This work is released under the MIT license.
//

#include "../include/Board.h"

#include <Eval.h>
#include <Node.h>
#include <catch2/catch_all.hpp>
#include <iostream>

auto root = Node();
std::string fen;

TEST_CASE("single undo") {
  SECTION("some move") {
    root.board()->import_fen("5k2/8/R4K2/8/8/8/8/8 w - - 0 1");
    root.board()->update_move_maps();
    root.board()->do_move(Square::a6, Square::a8, 0);
    CHECK(root.board()->export_fen() != "5k2/8/R4K2/8/8/8/8/8 w - - 0 1");
    root.board()->undo_move();
    CHECK(root.board()->export_fen() == "5k2/8/R4K2/8/8/8/8/8 w - - 0 1");
  }
  SECTION("promotion to queen") {
    root.board()->import_fen("5k2/P7/5K2/8/8/8/8/8 w - - 0 1");
    root.board()->update_move_maps();
    root.board()->do_move(Square::a7, Square::a8, 'q');
    CHECK(root.board()->export_fen() != "5k2/P7/5K2/8/8/8/8/8 w - - 0 1");
    root.board()->undo_move();
    CHECK(root.board()->export_fen() == "5k2/P7/5K2/8/8/8/8/8 w - - 0 1");
  }
}
TEST_CASE("multiple undoes") {
  SECTION("little opening") {

    root.board()->import_fen(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    root.board()->do_move(Square::e2, Square::e4, 0);
    root.board()->do_move(Square::e7, Square::e5, 0);
    root.board()->do_move(Square::g1, Square::f3, 0);
    root.board()->do_move(Square::b8, Square::c6, 0);
    root.board()->undo_move();
    root.board()->undo_move();
    root.board()->undo_move();
    CHECK(root.board()->export_fen() !=
          "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    root.board()->undo_move();
    CHECK(root.board()->export_fen() ==
          "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  }
  SECTION("with promotion") {
    root.board()->import_fen("8/7k/1P6/5K2/8/8/8/8 w - - 0 1");
    root.board()->do_move(Square::b6, Square::b7, 0);
    root.board()->do_move(Square::h7, Square::h6, 0);
    root.board()->do_move(Square::b7, Square::b8, 'q');
    root.board()->do_move(Square::h6, Square::h5, 0);
    root.board()->do_move(Square::b8, Square::h8, 0);
    root.board()->undo_move();
    root.board()->undo_move();
    root.board()->undo_move();
    root.board()->undo_move();
    CHECK(root.board()->export_fen() != "8/7k/1P6/5K2/8/8/8/8 w - - 0 1");
    root.board()->undo_move();
    CHECK(root.board()->export_fen() == "8/7k/1P6/5K2/8/8/8/8 w - - 0 1");
  }
}

TEST_CASE("Implied Tree Test") {
  root.board()->import_fen("5k2/8/R4K2/8/8/8/8/8 w - - 0 1");
  root.board()->update_move_maps();
  CHECK(Eval::eval(&root) > 0.0);
}