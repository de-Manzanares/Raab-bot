/*
 *     ____              __          __          __
 *    / __ \____ _____ _/ /_        / /_  ____  / /_
 *   / /_/ / __ `/ __ `/ __ \______/ __ \/ __ \/ __/
 *  / _, _/ /_/ / /_/ / /_/ /_____/ /_/ / /_/ / /_
 * /_/ |_|\__,_/\__,_/_.___/     /_.___/\____/\__/
 *
 * Copyright (c) 2024 de-Manzanares
 * This work is released under the MIT license.
 *
 */

#include "Node.h"
#include "Eval.h"

using s = Square;

// initialize static counter variables
uint Counter::node = 0;
std::chrono::time_point<std::chrono::high_resolution_clock> Counter::start =
    std::chrono::high_resolution_clock::now();

Node::Node() : _board(std::make_shared<Board>()) {
  _from = _to = s::h1;
  _promotion = 0;
}

Node::Node(const std::string &fen) : _board(std::make_shared<Board>()) {
  _board->import_fen(fen);
  _from = _to = s::h1;
  _promotion = 0;
}

Node::Node(const std::shared_ptr<Board> &board, const Square from,
           const Square to, const char ch)
    : _board(std::make_shared<Board>()) {
  *_board = *board;
  _from = from;
  _to = to;
  _promotion = ch;
  _board->do_move(from, to, ch);
}

uint Node::count_nodes() { // NOLINT
  uint count = _child.size();
  for (const auto &child : _child) {
    count += child->count_nodes();
  }
  return count;
}

void Node::spawn_depth_first(const uint depth) { // NOLINT
  if (depth == 0) {                              // terminal nodes
    _board->update_move_maps();
    _eval = Eval::eval(this);
    _board.reset();
    return;
  }

  _board->update_move_maps();

  // not at specified depth, but still a terminal node
  if (Eval::detect_stalemate_checkmate(this) != 2) { // 2 means neither
    _eval = Eval::eval(this);                        // stalemate nor checkmate
    _board.reset();
    return;
  }

  for (const auto &[sq, moves] : _board->game_state.active_color == Color::white
                                     ? _board->maps->white_moves
                                     : _board->maps->black_moves) {
    for (const auto &move : moves) {
      if ((_board->get_row(move) == 8 && _board->is_white_pawn(sq)) ||
          (_board->get_row(move) == 1 && _board->is_black_pawn(sq))) {
        for (const auto piece : std::vector{'q', 'r', 'b', 'n'}) {
          auto spawn = std::make_shared<Node>(Node(_board, sq, move, piece));
          spawn->_parent = this;
          _child.push_back(spawn);
          Counter::node++;
        }
      } else {
        auto spawn = std::make_shared<Node>(Node(_board, sq, move, 0));
        spawn->_parent = this;
        _child.push_back(spawn);
        Counter::node++;
      }
    }
  }

  // auto now = std::chrono::high_resolution_clock::now();
  // auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now
  // - Counter::start).count(); if (elapsed_ms > 15000) { return; }

  _board.reset();

  for (const auto &n : _child) {
    n->spawn_depth_first(depth - 1);
  }
}

std::shared_ptr<Node> Node::next_step(const std::shared_ptr<Node> &end) const {
  Node *current = end.get();
  while (current->_parent != this) {
    current = current->_parent;
  }
  return std::make_shared<Node>(*current);
}

uint Node::node_depth() const {
  uint ply = 0;
  auto current = this;
  while (current->_parent != nullptr) {
    current = current->_parent;
    ply++;
  }
  return ply;
}
