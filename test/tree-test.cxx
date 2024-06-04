#include <catch2/catch_all.hpp>
#include <iostream>
#include "../src/Board.hpp"
#include "../src/Tree.hpp"

TEST_CASE("creating a new node")
{
    SECTION("default") {
        Node n;
        CHECK(n._board.export_fen() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        CHECK(n._eval == 0);
    }SECTION("fen constructor") {
        Node n("1Q3k2/8/5K2/8/8/8/8/8 b - - 0 1");
        CHECK(n._eval == 1000);
    }SECTION("make a move constructor") {
        Board board;
        Node n(&board, Square::e2, Square::e4, 0);
        CHECK(n._board.export_fen() == "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
        CHECK(n._eval == 0);
    }SECTION("default constructor dynamic memory") {
        Node *n = new Node;
        CHECK(n->_board.export_fen() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        CHECK(n->_eval == 0);
    }SECTION("fen constructor dynamic memory") {
        Node *n = new Node("1Q3k2/8/5K2/8/8/8/8/8 b - - 0 1");
        CHECK(n->_eval == 1000);
    }SECTION("make a move constructor dynamic memory") {
        Board board;
        Node *n = new Node(&board, Square::e2, Square::e4, 0);
        CHECK(n->_board.export_fen() == "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
        CHECK(n->_eval == 0);
    }SECTION("make a move constructor dynamic memory non startpos initial") {
        Board board;
        board.import_fen("5k2/1Q6/5K2/8/8/8/8/8 w - - 0 1");
        Node *n = new Node(&board, Square::b7, Square::b8, 0);
        CHECK(n->_board.export_fen() == "1Q3k2/8/5K2/8/8/8/8/8 b - - 1 1");
        CHECK(n->_eval == 1000);
    }
}

TEST_CASE("spawn")
{
    SECTION("checkmate") {
        Node n("1Q3k2/8/5K2/8/8/8/8/8 b - - 1 1");
        n.spawn(1);
        CHECK(n._child.size() == 0);
    }SECTION("one possible move") {
        Node n("1Q4k1/7N/5K2/8/8/8/8/8 b - - 0 1");
        CHECK(n._eval == 12);
        n.spawn(1);
        CHECK(n._eval == 12);
        CHECK(n._child.size() == 1);
        CHECK(n._child[0]->_board.export_fen() == "1Q6/7k/5K2/8/8/8/8/8 w - - 0 2");
        CHECK(n._child[0]->_eval == 9);
        CHECK(n._eval == 12);
    }SECTION("mate in one") {
        Node n("6k1/3PR3/4P1K1/8/8/8/8/8 w - - 0 1");
        n.spawn(1);
        CHECK(n._child.size() == 13);
        std::vector<std::string> path;
        for (const auto& m : path) {
            std::cout << m << " ";
        }
        std::cout << "\n";
    }SECTION("mate in two") {
        Node n("7k/5ppp/1P4p1/8/7K/8/8/8 w - - 0 1");
        n.spawn(3);
        std::cout << n.count_nodes() << "\n";
    }
}
