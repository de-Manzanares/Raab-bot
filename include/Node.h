/*
 *     ____              __          __          __
 *    / __ \____ _____ _/ /_        / /_  ____  / /_
 *   / /_/ / __ `/ __ `/ __ \______/ __ \/ __ \/ __/
 *  / _, _/ /_/ / /_/ / /_/ /_____/ /_/ / /_/ / /_
 * /_/ |_|\__,_/\__,_/_.___/     /_.___/\____/\__/
 *
 * Copyright (c) 2024 de-Manzanares
 * This work is released under the MIT license.
 */

#ifndef INCLUDE_NODE_H_
#define INCLUDE_NODE_H_

#include "Board.h"

#include <chrono>
#include <memory>
#include <string>
#include <vector>

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
  /// @brief Default constructor for Node class
  Node();

  /**
   * @brief Create a Node with the given FEN string.
   * @param fen The FEN string representing a game state
   */
  explicit Node(const std::string &fen);

  /**
   * @brief Constructor used in tree generation
   * @param board The board to be copied.
   * @param from The starting square of the move to be made.
   * @param to The ending square of the move to be made.
   * @param ch If the move is a pawn promotion, the character to promote to.
   */
  Node(const std::shared_ptr<Board> &board, Square from, Square to, char ch);

  /// @brief Destructor for the Node class
  ~Node();

  Square _from; ///< from Square of move that created this position
  Square _to;   ///< to Square of move that created this position
  char _ch;     ///< if a pawn is promoting, this gives the piece to promote to
  Node *parent{};                  ///< parent node
  std::shared_ptr<Board> _board{}; ///< Board
  double _eval{};                  ///< evaluation
  std::vector<Node *> _child{};    ///< children nodes

  /**
   * @brief Creates a decision tree of n layers.
   * @details Uses a depth-first recursive algorithm
   * @param depth The depth of the tree to spawn child nodes for.
   * @note Resets (deletes) boards as additional layers are spawned. Don't try
   * to access boards after creating the tree: they won't be there.
   */
  void spawn_depth_first(uint depth);

  void spawn_breadth_first(uint depth); ///< Create a decision tree

  /**
   * @brief Counts all the nodes below the calling node
   * @return The number of nodes in the tree "below" the calling node.
   */
  uint count_nodes();

  /**
   * @brief Finds the next node between this node and the target node
   * @param end The target node
   * @return The next node between this node and the target node
   * @warning Target node must be a child of the calling node ... otherwise :~(
   */
  Node *next_step(Node *end) const;

  /**
   * @brief Finds the next node between this node and the target node
   * @param end The target node
   * @param depth To track distance between nodes
   * @return The next node between this node and the target node
   * @warning Target node must be a child of the calling node ... otherwise :~(
   */
  Node *next_step(Node *end, uint *depth) const;

  /**
   * @return The distance in ply from a this node to the root node.
   */
  [[nodiscard]] uint node_depth() const;
};

#endif // INCLUDE_NODE_H_
