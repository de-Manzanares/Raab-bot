#ifndef CHESSENGINE_TREE_HPP
#define CHESSENGINE_TREE_HPP

#include <limits>
#include "Board.hpp"

struct Node;
struct Tree {
    std::vector<std::vector<Node *>> rings{};
};

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

double mobility_evaluation(const Board *board)
{
    const double MULTIPLIER = 1.0 / 100.0;
    double score = 0;
    for (const auto& [sq, moves] : board->move_map_white) {
        score += static_cast<double>(moves.size());
    }
    for (const auto& [sq, moves] : board->move_map_black) {
        score -= static_cast<double>(moves.size());
    }
    return score * MULTIPLIER;
}

double check_bonus(const Board *board)
{
    const double VALUE = 0.5;
    double score = 0;
    if (board->game_state.in_check_white) { score -= VALUE; }
    if (board->game_state.in_check_black) { score += VALUE; }
    return score;
}

double simple_evaluation(const Board *board)
{
    if (detect_checkmate(board) == 0) {
        double sum = material_evaluation(board)
                + mobility_evaluation(board)
                + check_bonus(board);
        return sum;
    }
    return 1'000 * detect_checkmate(board);
}

double eval(const Board *board)
{
    return simple_evaluation(board);
}

struct Node {
    Node();
    explicit Node(const std::string& fen);
    Node(const Board *board, Square from, Square to, char ch);

    ~Node();

    std::string _move{};
    Node *parent{};
    Board _board{};
    double _eval{};
    std::vector<Node *> _child{};
    uint count_nodes();

    void spawn(uint depth);

    Node *next_step(Node *start, Node *end);
    Node *next_step(Node *end);
};

Node::Node()
{
    _board.update_move_maps();
    _eval = eval(&_board);
}

Node::Node(const std::string& fen)
{
    _board.import_fen(fen);
    _board.update_move_maps();
    _eval = eval(&_board);
}

Node::Node(const Board *board, Square from, Square to, char ch)
{
    _board = *board;
    _board.move(from, to, ch);
    _board.update_move_maps();
    _eval = eval(&_board);
    _move += square_to_string(from) += square_to_string(to);
    if (ch != 0) {
        _move += ch;
    }
}

Node::~Node()
{
    for (auto& child : _child) {
        delete child;
        child = nullptr;
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



// todo untangle later

void Node::spawn(uint depth)
{
    if (depth == 0) { return; }

    for (const auto& [sq, moves] :
            _board.game_state.active_color == Color::white ?
            _board.move_map_white : _board.move_map_black) {

        for (const auto& move : moves) {
            if ((_board.is_in_row(move) == 8 && _board.is_white_pawn(sq))
                    || _board.is_in_row(move) == 1 && _board.is_black_pawn(sq)) {
                std::vector<char> promotions{'q', 'r', 'b', 'n'};
                for (auto piece : promotions) {
                    Node *spawn = new Node(&_board, sq, move, piece);
                    spawn->parent = this;
                    _child.push_back(spawn);
                    if (detect_checkmate(&spawn->_board) != 0) { return; }   // i'm not sure what this does lol
                }
            }
            else {
                Node *spawn = new Node(&_board, sq, move, 0);
                spawn->parent = this;
                _child.push_back(spawn);
                if (detect_checkmate(&spawn->_board) != 0) { return; }   // i'm not sure what this does lol
            }
        }
        // std::cout << "info " << count_nodes() << " nodes" << "\n";
    }
    // populate rings
    // assign parent node
    for (auto& n : _child) {
        n->spawn(depth - 1);
    }
}

///@warning infinite loop if start node doesn't lead to end node
Node *Node::next_step(Node *end)
{
    Node *current = end;
    while (current->parent != this) { current = current->parent; }
    return current;
}

struct Evaluator {
    std::string best_move(const Node *n);
    void best_line(const Node *n, uint depth);
    Node *best_node;
    std::string line{};
};

std::string Evaluator::best_move(const Node *n)
{
    std::string best_move;

    bool maximizing = n->_board.game_state.active_color == Color::white;
    double best_eval =
            maximizing ? -std::numeric_limits<double>::infinity() : std::numeric_limits<double>::infinity();
    for (const auto& c : n->_child) {
        if (maximizing && c->_eval > best_eval || !maximizing && c->_eval < best_eval) {
            best_eval = c->_eval;
            best_move = c->_move;
            best_node = c;
        }
    }
    return best_move;
}

void Evaluator::best_line(const Node *n, uint depth)
{
    line.clear();
    line += best_move(n);
    line += ' ';
    for (auto i = depth; i > 0; --i) {
        line += best_move(best_node);
        line += ' ';
    }
    std::cout << line;
}

Node *max(std::vector<Node *> v)
{
    Node *max_node;
    double max = -std::numeric_limits<double>::infinity();
    for (const auto& n : v) {
        if (n->_eval > max) {
            max = n->_eval;
            max_node = n;
        }
    }
    return max_node;
}

Node *min(std::vector<Node *> v)
{
    Node *min_node;
    double min = std::numeric_limits<double>::infinity();
    for (const auto& n : v) {
        if (n->_eval < min) {
            min = n->_eval;
            min_node = n;
        }
    }
    return min_node;
}

std::vector<Node *> minmax(std::vector<std::vector<Node *>> v, uint depth)
{
    std::vector<Node *> path;
    Color c = v[v.size() - 1][0]->_board.game_state.active_color;

    for (auto i = depth - 1; depth >= 0; depth--) {
        if (c == Color::white) { path.push_back(max(v[i])); }
        else if (c == Color::black) { path.push_back(min(v[i])); }
        // switch turn
        !c;
    }
    return path;
}

void print_nodes(std::vector<Node *> path)
{
    std::reverse(path.begin(), path.end());
    for (const auto& n : path) {
        std::cout << n->_move << " ";
    }
    std::cout << "\n";
}


// if it is white's turn, white is going to choose the highest number
// if it is black's turn, black is going to choose the lowest number

#endif //CHESSENGINE_TREE_HPP
