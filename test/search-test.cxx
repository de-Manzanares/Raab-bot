#include <catch2/catch_all.hpp>
#include <iostream>
#include "../src/Board.hpp"
#include "../src/tree.hpp"
#include "../src/search.hpp"

double neg_inf = -std::numeric_limits<double>::infinity();
double pos_inf = std::numeric_limits<double>::infinity();

// TODO there is something wrong with my tree generation... it is taking too long...
// TODO maybe if we enhance the evaluation, the alpha beta search will be more effective LOL
// ALL of the time is in generating the tree ...

TEST_CASE("search")
{
    SECTION("mate in one") {
        Node n("6k1/2R5/6K1/8/8/8/8/8 w - - 0 1");
        n.spawn(2);                                 // we need to get the nodes to stop spawning if game is over
        auto step_1 = min_max(&n, 2, neg_inf, pos_inf, true);
        std::cout << "mate in one nodes: " << n.count_nodes() << "\n";
        std::cout << step_1.second->_move << "\n";
        CHECK(step_1.first == 1000);        // found the mate in one
    }SECTION("mate in two") {
        Node n("8/6k1/2R5/1R4K1/8/8/5P2/8 w - - 0 1");
        n.spawn(2);
        auto step_1 = min_max(&n, 2, neg_inf, pos_inf, true);
        std::cout << "step 1 nodes: " << n.count_nodes() << "\n";
        std::cout << "step 1 best move: " << step_1.second->parent->_move << "\n";

        step_1.second->spawn(2);
        auto step_2 = min_max(step_1.second, 2, neg_inf, pos_inf, true);
        std::cout << "step 2 nodes: " << step_1.second->count_nodes() << "\n";
        std::cout << "step 2 best move: " << step_2.second->parent->_move << "\n";

        step_2.second->spawn(2);
        auto step_3 = min_max(step_2.second, 2, neg_inf, pos_inf, true);
        std::cout << "step 3 nodes: " << step_3.second->count_nodes() << "\n";
        std::cout << "step 3 best move: " << step_3.second->parent->_move << "\n";
        for (const auto& c : step_2.second->_child) {
            std::cout << c->_eval << " : " << c->_move << "\n";
        }
    }

    // SECTION("mate in three") {                     // too slow
    //    Node n("8/8/6k1/2R5/1R4K1/8/8/8 w - - 0 1");
    //    n.spawn(5);
    //    std::cout << "mate in three nodes: " << n.count_nodes() << "\n";
    //    CHECK(min_max(&n, 5, neg_inf, pos_inf, true) == 1000);        // found the mate in three
    //}
}