#ifndef CHESSENGINE_TREE_HPP
#define CHESSENGINE_TREE_HPP

#include "Board.hpp"
#include "Evaluator.hpp"

std::unordered_map<char, int> material_value = {
        {'Q', 900},
        {'q', -900},
        {'R', 500},
        {'r', -500},
        {'B', 310},
        {'b', -310},
        {'N', 300},
        {'n', -300},
        {'P', 100},
        {'p', -100}};

double material_evaluation(const Board *board)
{
    double sum = 0;
    for (Square sq = Square::a8; sq >= Square::h1; --sq) {
        if (!board->is_empty(sq)) {
            sum += material_value[board->what_piece(sq)];
        }
    }
    return sum / 100;
}

int detect_checkmate(const Board *board)
{
    ulong number_of_moves = 0;

    // detect white checkmate
    if (board->game_state.active_color == Color::white) {
        for (const auto& [sq, moves] : board->move_map_white) { number_of_moves += moves.size(); }
        if (number_of_moves == 0) { return -1; }
    }

    // detect black checkmate
    if (board->game_state.active_color == Color::black) {
        for (const auto& [sq, moves] : board->move_map_black) { number_of_moves += moves.size(); }
        if (number_of_moves == 0) { return 1; }
    }

    // if neither is in checkmate
    return 0;
}

double simple_evaluation(const Board *board)
{
    if (detect_checkmate(board) == 0) { return material_evaluation(board); }
    return 1'000 * detect_checkmate(board);
}

double eval(const Board *board)
{
    return simple_evaluation(board);
}

struct Node {
    Node();
    Node(const std::string fen);
    Node(Board board, Square from, Square to, char ch);

    ~Node();

    std::string _move{};
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

Node::Node(Board board, Square from, Square to, char ch)
{
    _board = board;
    _board.move(from, to, 0);
    _board.update_move_maps();
    _eval = eval(&_board);
    _move += square_to_string(from) += square_to_string(to);
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
                    Node *spawn = new Node(_board, sq, move, piece);
                    _child.push_back(spawn);
                }
            }
            else {
                Node *spawn = new Node(_board, sq, move, 0);
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

std::vector<std::string> min_max(const Node *root, uint depth)
{
    std::vector<std::string> path;

    // TODO deal with tied evaluations

    bool maximizing = root->_board.game_state.active_color == Color::white;
    double best_eval = (maximizing ?
                        -std::numeric_limits<double>::infinity() : std::numeric_limits<double>::infinity());
    std::string best_move;
    std::vector<std::string> best_path;

    for (const auto& child : root->_child) {
        double child_eval = child->_eval;

        if ((maximizing && child_eval > best_eval)
                || !maximizing && child_eval < best_eval) {
            best_eval = child_eval;
            best_move = child->_move;
        }
    }
    path.push_back(best_move);
    return path;
}

// if it is white's turn, white is going to choose the highest number
// if it is black's turn, black is going to choose the lowest number

#endif //CHESSENGINE_TREE_HPP
