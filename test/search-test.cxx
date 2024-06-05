#include <catch2/catch_all.hpp>
#include <iostream>
#include "../src/Board.hpp"
#include "../src/tree.hpp"
#include "../src/search.hpp"


// TODO there is something wrong with my tree generation... it is taking too long...
// TODO maybe if we enhance the evaluation, the alpha beta search will be more effective LOL
// ALL of the time is in generating the tree ...

std::string delim = "\n--------------------------------------------------\n\n";

bool is_maxing(Node *n)
{
    return n->_board.game_state.active_color == Color::white;
}

const uint D = 4;   // depth
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
    moves.push_back(n.next_step(opt_nodes[0]));
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
    moves.push_back(n.next_step(opt_nodes[0]));
    std::cout << moves[0]->_move << "\n";

    for (int i = 0; i < 2; i++) {
        opt_nodes[i]->spawn(D);
        opt_nodes.push_back(min_max(opt_nodes[i], D, neg_inf, pos_inf, is_maxing(opt_nodes[i])));
        moves.push_back(moves[i]->next_step(opt_nodes[i + 1]));
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
    moves.push_back(n.next_step(opt_nodes[0]));
    std::cout << moves[0]->_move << "\n";

    for (int i = 0; i < 2; i++) {
        opt_nodes[i]->spawn(D);
        opt_nodes.push_back(min_max(opt_nodes[i], D, neg_inf, pos_inf, is_maxing(opt_nodes[i])));
        moves.push_back(moves[i]->next_step(opt_nodes[i + 1]));
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
    moves.push_back(n.next_step(opt_nodes[0]));
    std::cout << moves[0]->_move << "\n";

    for (int i = 0; i < 4; i++) {
        opt_nodes[i]->spawn(D);
        opt_nodes.push_back(min_max(opt_nodes[i], D, neg_inf, pos_inf, is_maxing(opt_nodes[i])));
        moves.push_back(moves[i]->next_step(opt_nodes[i + 1]));
        std::cout << moves[i + 1]->_move << "\n";
    }
}
