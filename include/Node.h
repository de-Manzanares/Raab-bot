#ifndef INCLUDE_NODE_H_
#define INCLUDE_NODE_H_

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "Board.h"
#include "Eval.h"

/**
 * @struct Counter
 * @brief Tracks node count and time points for giving infos
 */
struct Counter {
  static uint node; ///< Tracks the number of nodes in the tree
  static std::chrono::time_point<std::chrono::high_resolution_clock>
      start; ///< For calculating elapse and rates
};

/**
 * @class Node
 * @brief Represents a node in a tree structure
 * @details Each node contains a possible game state.
 */
struct Node {
  Node();
  explicit Node(const std::string &fen);
  Node(const std::shared_ptr<Board> &board, Square from, Square to, char ch);
  ~Node();

  Square _from; ///< from Square of move that created this position
  Square _to;   ///< to Square of move that created this position
  char _ch;     ///< if a pawn is promoting, this gives the piece to promote to
  Node *parent{};                  ///< parent node
  std::shared_ptr<Board> _board{}; ///< Board
  double _eval{};                  ///< evaluation
  std::vector<Node *> _child{};    ///< children nodes

  void spawn_depth_first(uint depth);
  void spawn_breadth_first(uint depth); ///< Create a decision tree

  uint count_nodes();
  Node *next_step(Node *end, uint *depth);
  uint node_depth();
};

#endif // INCLUDE_NODE_H_
