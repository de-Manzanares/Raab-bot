#ifndef CHESSENGINE_TREE_HPP
#define CHESSENGINE_TREE_HPP

#include "Board.hpp"
#include "Evaluator.hpp"

struct Node {
    Node();
    Node(const std::string fen);
    Node(const Board *board, Square from, Square to, char ch);

    ~Node();

    Node *parent{};
    Board _board{};
    double _eval{};
    std::vector<Node *> _child{};
    uint count_nodes();

    void spawn(uint depth);
};

Node::Node()
{
    _board.update_move_maps();
    _eval = eval(&_board);
}

Node::Node(const std::string fen)
{
    _board.import_fen(fen);
    _board.update_move_maps();
    _eval = eval(&_board);
}

Node::Node(const Board *board, Square from, Square to, char ch)
{
    _board = *board;
    _board.move(from, to, 0);
    _board.update_move_maps();
    _eval = eval(&_board);
}

Node::~Node()
{
    for (auto& child : _child) {
        delete child;
        child = nullptr;
    }
}

void Node::spawn(uint depth)
{
    if (depth == 0) { return; }

    for (const auto& [sq, moves] :
            _board.game_state.active_color == Color::white ? _board.move_map_white : _board.move_map_black) {
        for (const auto& move : moves) {
            if ((_board.is_in_row(move) == 8 && _board.is_white_pawn(sq))
                    || _board.is_in_row(move) == 1 && _board.is_black_pawn(sq)) {
                std::vector<char> promotions{'q', 'r', 'b', 'n'};
                for (auto piece : promotions) {
                    Node *spawn = new Node(&_board, sq, move, piece);
                    _child.push_back(spawn);
                }
            }
            else {
                Node *spawn = new Node(&_board, sq, move, 0);
                _child.push_back(spawn);
            }
        }
    }

    // assign parent node
    for (auto& n : _child) {
        n->parent = this;
        n->spawn(depth - 1);
    }
}

uint Node::count_nodes()
{
    uint count = _child.size();
    for (const auto& child : _child) {
        count += child->count_nodes();
    }
    return count;
}

#endif //CHESSENGINE_TREE_HPP
