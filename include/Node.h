#ifndef CHESSENGINE_NODE_H
#define CHESSENGINE_NODE_H

#include <cstdint>
#include <chrono>

#include "Eval.h"
#include "Board.h"

// TODO make nodes as small as possible
// TODO get rid of move, redundant with _from and _to

struct Counter {
    static uint node;
    static std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

/**
 * @class Node
 * @brief Represents a node in a tree structure
 * @details Each node contains a possible game state.
 */
struct Node {
    Node();
    explicit Node(const std::string& fen);
    Node(const Board *board, Square from, Square to, char ch);
    ~Node();

    Square _from;
    Square _to;
    std::string _move{};
    Node *parent{};
    Board _board{};
    double _eval{};
    std::vector<Node *> _child{};

    uint count_nodes();
    void spawn_depth_first(uint depth);
    Node *next_step(Node *end, uint *depth);
    uint node_depth();
    void spawn_breadth_first(uint depth);
};

#endif //CHESSENGINE_NODE_H
