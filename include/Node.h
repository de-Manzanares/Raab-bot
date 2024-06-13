#ifndef INCLUDE_NODE_H_
#define INCLUDE_NODE_H_

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include "Eval.h"
#include "Board.h"

// TODO make nodes as small as possible
// TODO get rid of move, redundant with _from and _to

/**
 * @struct Counter
 * @brief Tracks node count and time points for giving info's
 */
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
    Node(const std::shared_ptr<Board>& board, Square from, Square to, char ch);
    ~Node();

    Square _from;                   // tracks the move that was made between the parent node and this node
    Square _to;                     // tracks the move that was made between the parent node and this node
    std::string _move{};            // tracks the move that was made between the parent node and this node
    Node *parent{};                 // parent node
    // TODO may have to allocate memory lol
    std::shared_ptr<Board> _board{};                // board
    double _eval{};                 // evaluation
    std::vector<Node *> _child{};   // vector of nodes spawned from this node

    uint count_nodes();
    void spawn_depth_first(uint depth);
    Node *next_step(Node *end, uint *depth);
    uint node_depth();
    void spawn_breadth_first(uint depth);
};

#endif  // INCLUDE_NODE_H_
