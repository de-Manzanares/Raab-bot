#include "Node.h"
#include "Eval.h"

using s = Square;

// initialize static counter variables
uint Counter::node = 0;
std::chrono::time_point<std::chrono::high_resolution_clock> Counter::start =
    std::chrono::high_resolution_clock::now();

Node::Node() : _board(std::make_shared<Board>()) {
  _from = _to = s::h1;
  _ch = 0;
}

Node::Node(const std::string &fen) : _board(std::make_shared<Board>()) {
  _board->import_fen(fen);
  _from = _to = s::h1;
  _ch = 0;
}

Node::Node(const std::shared_ptr<Board> &board, const Square from,
           const Square to, const char ch)
    : _board(std::make_shared<Board>()) {
  *_board = *board;
  _from = from;
  _to = to;
  _ch = ch;
  _board->do_move(from, to, ch);
}

// TODO make node shared ptr
Node::~Node() {
  for (auto &child : _child) {
    delete child;
  }
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

  if (Eval::detect_checkmate(this) !=
      0) { // not at specified depth, but still a terminal node
    _eval = Eval::eval(this);
    _board.reset();
    return;
  }

  for (const auto &[sq, moves] : _board->game_state.active_color == Color::white
                                     ? _board->maps->white_moves
                                     : _board->maps->black_moves) {
    for (const auto &move : moves) {
      if ((_board->get_row(move) == 8 && _board->is_white_pawn(sq)) ||
          _board->get_row(move) == 1 && _board->is_black_pawn(sq)) {
        for (const auto piece : std::vector{'q', 'r', 'b', 'n'}) {
          auto spawn = new Node(_board, sq, move, piece);
          spawn->parent = this;
          _child.push_back(spawn);
          Counter::node++;
        }
      } else {
        auto spawn = new Node(_board, sq, move, 0);
        spawn->parent = this;
        _child.push_back(spawn);
        Counter::node++;
      }
    }
  }

  // auto now = std::chrono::high_resolution_clock::now();
  // auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now
  // - Counter::start).count(); if (elapsed_ms > 15000) { return; }

  _board.reset();

  for (auto &n : _child) {
    n->spawn_depth_first(depth - 1);
  }
}

Node *Node::next_step(Node *end, uint *depth) const {
  Node *current = end;
  while (current->parent != this) {
    current = current->parent;
    (*depth)++;
  }
  return current;
}

uint Node::node_depth() const {
  uint ply = 0;
  auto current = this;
  while (current->parent != nullptr) {
    current = current->parent;
    ply++;
  }
  return ply;
}
