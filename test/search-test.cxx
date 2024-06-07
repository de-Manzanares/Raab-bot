#include <catch2/catch_all.hpp>
#include <iostream>
#include "../src/Board.hpp"
#include "../src/tree.hpp"
#include "../src/search.hpp"

uint placeholder{};

std::string delim = "\n--------------------------------------------------\n\n";

bool is_maxing(Node *n)
{
    return n->_board.game_state.active_color == Color::white;
}

uint D = 2;   // depth
double neg_inf = -std::numeric_limits<double>::infinity();
double pos_inf = std::numeric_limits<double>::infinity();

TEST_CASE("mate in one")
{
    std::cout << delim;
    Node n("6k1/2R5/6K1/8/8/8/8/8 w - - 0 1");
    n.spawn(D);
    std::vector<Node *> opt_nodes;
    std::vector<Node *> moves;
    opt_nodes.push_back(min_max(&n, D, neg_inf, pos_inf, is_maxing(&n)));
    moves.push_back(n.next_step(opt_nodes[0], &placeholder));
    std::cout << moves[0]->_move;
}

TEST_CASE("mate in two")
{
    std::cout << delim;
    Node n("8/7k/1R6/2R3K1/8/8/8/8 w - - 0 1");
    std::vector<Node *> opt_nodes;
    std::vector<Node *> moves;

    n.spawn(D);
    opt_nodes.push_back(min_max(&n, D, neg_inf, pos_inf, is_maxing(&n)));
    moves.push_back(n.next_step(opt_nodes[0], &placeholder));
    std::cout << moves[0]->_move << "\n";

    for (int i = 0; i < 2; i++) {
        opt_nodes[i]->spawn(D);
        opt_nodes.push_back(min_max(opt_nodes[i], D, neg_inf, pos_inf, is_maxing(opt_nodes[i])));
        moves.push_back(moves[i]->next_step(opt_nodes[i + 1], &placeholder));
        std::cout << moves[i + 1]->_move << "\n";
    }
}

TEST_CASE("mate in two, part 2")
{
    std::cout << delim;
    Node n("8/8/7k/2R5/1R4K1/8/8/8 w - - 0 1");
    std::vector<Node *> opt_nodes;
    std::vector<Node *> moves;

    n.spawn(D);
    opt_nodes.push_back(min_max(&n, D, neg_inf, pos_inf, is_maxing(&n)));
    moves.push_back(n.next_step(opt_nodes[0], &placeholder));
    std::cout << moves[0]->_move << "\n";

    for (int i = 0; i < 2; i++) {
        opt_nodes[i]->spawn(D);
        opt_nodes.push_back(min_max(opt_nodes[i], D, neg_inf, pos_inf, is_maxing(opt_nodes[i])));
        moves.push_back(moves[i]->next_step(opt_nodes[i + 1], &placeholder));
        std::cout << moves[i + 1]->_move << "\n";
    }
}

TEST_CASE("mate in three")      // has a real round about solution LOL
{
    std::cout << delim;
    Node n("8/8/6k1/2R5/1R4K1/8/8/8 w - - 0 1");
    std::vector<Node *> opt_nodes;
    std::vector<Node *> moves;

    n.spawn(D);
    opt_nodes.push_back(min_max(&n, D, neg_inf, pos_inf, is_maxing(&n)));
    moves.push_back(n.next_step(opt_nodes[0], &placeholder));
    std::cout << moves[0]->_move << "\n";

    for (int i = 0; i < 4; i++) {
        opt_nodes[i]->spawn(D);
        opt_nodes.push_back(min_max(opt_nodes[i], D, neg_inf, pos_inf, is_maxing(opt_nodes[i])));
        moves.push_back(moves[i]->next_step(opt_nodes[i + 1], &placeholder));
        std::cout << moves[i + 1]->_move << "\n";
    }
}

TEST_CASE("hanging piece ct#50966377")
{
    std::cout << delim;
    Node n("3b4/5q1k/pp2R3/P3Pprp/1P6/3Q1K1B/8/8 w - - 0 1");
    std::vector<Node *> opt_nodes;
    std::vector<Node *> moves;

    n.spawn(D);
    opt_nodes.push_back(min_max(&n, D, neg_inf, pos_inf, is_maxing(&n)));
    moves.push_back(n.next_step(opt_nodes[0], &placeholder));
    std::cout << moves[0]->_move << "\n";

    for (int i = 0; i < 4; i++) {
        opt_nodes[i]->spawn(D);
        opt_nodes.push_back(min_max(opt_nodes[i], D, neg_inf, pos_inf, is_maxing(opt_nodes[i])));
        moves.push_back(moves[i]->next_step(opt_nodes[i + 1], &placeholder));
        std::cout << moves[i + 1]->_move << "\n";
    }
}

TEST_CASE("hanging piece ct#50966377 modified lol white first")
{
    D = 2;
    std::cout << delim;
    Node n("3b4/5q1k/pp6/P3Pprp/1P6/3Q1K1B/8/8 w - - 0 1");
    std::vector<Node *> opt_nodes;
    std::vector<Node *> moves;

    n.spawn(D);
    opt_nodes.push_back(min_max(&n, D, neg_inf, pos_inf, is_maxing(&n)));
    moves.push_back(n.next_step(opt_nodes[0], &placeholder));
    std::cout << moves[0]->_move << "\n";

    for (int i = 0; i < 4; i++) {
        opt_nodes[i]->spawn(D);
        opt_nodes.push_back(min_max(opt_nodes[i], D, neg_inf, pos_inf, is_maxing(opt_nodes[i])));
        moves.push_back(moves[i]->next_step(opt_nodes[i + 1], &placeholder));
        std::cout << moves[i + 1]->_move << "\n";
    }
}

TEST_CASE("hanging piece ct#50966377 modified lol black first")
{
    D = 2;
    std::cout << delim;
    Node n("3b4/5q1k/pp6/P3Pprp/1P6/3Q1K1B/8/8 b - - 0 1");
    std::vector<Node *> opt_nodes;
    std::vector<Node *> moves;

    n.spawn(D);
    opt_nodes.push_back(min_max(&n, D, neg_inf, pos_inf, is_maxing(&n)));
    moves.push_back(n.next_step(opt_nodes[0], &placeholder));
    std::cout << moves[0]->_move << "\n";

    for (int i = 0; i < 4; i++) {
        opt_nodes[i]->spawn(D);
        opt_nodes.push_back(min_max(opt_nodes[i], D, neg_inf, pos_inf, is_maxing(opt_nodes[i])));
        moves.push_back(moves[i]->next_step(opt_nodes[i + 1], &placeholder));
        std::cout << moves[i + 1]->_move << "\n";
    }
}

TEST_CASE("depth three gives exit code -9")
{
    Node n("r1b1kb1r/ppp2ppp/2np4/3Pp3/2P1q3/4P3/PP3PPP/RNBQK1NR w KQkq - 0 9");
    std::vector<uint> w{0};
    std::vector<uint> b{0};

    for (const auto& [sq, moves] : n._board.move_map_white) {
        w[0] += moves.size();
    }
    for (const auto& [sq, moves] : n._board.move_map_black) {
        b[0] += moves.size();
    }

    std::cout << w[0] << "\n" << b[0] << "\n";
    std::cout << "est depth 2: " << w[0] * b[0] << " nodes\n";
    std::cout << "est depth 3: " << w[0] * b[0] * w[0] << " nodes\n";
    std::cout << "est depth 4: " << w[0] * b[0] * w[0] * b[0] << " nodes\n";
    // n.spawn(D);
}