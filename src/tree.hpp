#ifndef CHESSENGINE_TREE_HPP
#define CHESSENGINE_TREE_HPP

#include <limits>
#include "Board.hpp"

// TODO do you have a memory leak?!
// TODO iterative deepening
// TODO enable partial analysis - some depth 3 is better than no depth 3
// spawn based on time left and complexity (predicted nodes)
// TODO discourage stalemate (worse than winning better than losing)
// passed pawns good
// stacked pawns bad
// TODO move order
// TODO include influence as well as legal moves (instead of ?)
// bonus for influence around enemy king?

// TODO checks seem to cause freeze ups
// TODO local opening book
// TODO local table-base
// TODO avoid pawn forks LOL
// TODO influencing the center

struct Node;
double discourage_early_queen_movement(const Node *node);
double castle_bonus(Node *n);

const double CHECK_BONUS = 0.25;                     // gives weight to checks 0.25
const double MOBILITY_MULTIPLIER = 1.0 / 100.0;     // gives weight for each legal move 1.0
const double CASTLE_BONUS = 0.5;                    // gives weight to castling 0.5

/// material value
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

/**
 * @brief Evaluates the material value on the given chess board.
 * @details This function calculates the material value of the chess pieces on the given chess board.
 * @param board A pointer to the chess board to be evaluated.
 * @return The material value of the chess pieces on the board divided by 100.
 */
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

/**
 * @brief Detects if the given chess board is in a checkmate position.
 * @detlails Check whether there are any legal moves available for the active player. If there are no
 * legal moves available, this is checkmate.
 * @param board The chess board for which to detect checkmate.
 * @return -1 if White is in checkmate, 1 if Black is in checkmate, 0 if neither color is in checkmate.
 */
int detect_checkmate(const Board *board)
{
    ulong number_of_moves = 0;

    // detect white checkmate
    if (board->game_state.active_color == Color::white) {
        for (const auto& [sq, moves] : board->maps->move_map_white) { number_of_moves += moves.size(); }
        if (number_of_moves == 0) { return -1; }
    }
    // detect black checkmate
    if (board->game_state.active_color == Color::black) {
        for (const auto& [sq, moves] : board->maps->move_map_black) { number_of_moves += moves.size(); }
        if (number_of_moves == 0) { return 1; }
    }
    // if neither is in checkmate
    return 0;
}

/**
 * @brief Evaluates the mobility of a given chess board.
 * @details Adds score for each legal move available to a side.
 * @param board A pointer to the chess board.
 * @return The mobility score.
 */
double mobility_evaluation(const Board *board)
{
    double score = 0;
    for (const auto& [sq, moves] : board->maps->move_map_white) {
        score += static_cast<double>(moves.size());
    }
    for (const auto& [sq, moves] : board->maps->move_map_black) {
        score -= static_cast<double>(moves.size());
    }
    return score * MOBILITY_MULTIPLIER;
}

/**
 * Calculates the bonus score for a given board based on whether a player is in check.
 * @param board The board to check for in-check status.
 * @return (+) score is black is in check, (-) score if white is in check
 */
double check_bonus(const Board *board)
{
    double score = 0;
    if (board->game_state.in_check_white) { score -= CHECK_BONUS; }
    if (board->game_state.in_check_black) { score += CHECK_BONUS; }
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

// TODO make nodes as small as possible

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

    void spawn(uint depth);

    Node *next_step(Node *end, uint *depth);
};

Node::Node()
{
    _board.update_move_maps();
    _eval = eval(&_board) + discourage_early_queen_movement(this) + castle_bonus(this);
}

Node::Node(const std::string& fen)
{
    _board.import_fen(fen);
    _board.update_move_maps();
    _eval = eval(&_board) + discourage_early_queen_movement(this) + castle_bonus(this);
}

Node::Node(const Board *board, Square from, Square to, char ch)
{
    _board = *board;
    _from = from;
    _to = to;
    _board.move(from, to, ch);
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

void Node::spawn(uint depth)
{
    if (depth == 0) {
        _board.update_move_maps();
        _eval = eval(&_board) + discourage_early_queen_movement(this) + castle_bonus(this);
        delete _board.maps;
        _board.maps = nullptr;
        return;
    }

    _board.update_move_maps();
    _eval = eval(&_board) + discourage_early_queen_movement(this) + castle_bonus(this);

    if (detect_checkmate(&_board) != 0) { return; }

    for (const auto& [sq, moves] :
            _board.game_state.active_color == Color::white ?
            _board.maps->move_map_white : _board.maps->move_map_black) {

        for (const auto& move : moves) {
            if ((_board.is_in_row(move) == 8 && _board.is_white_pawn(sq))
                    || _board.is_in_row(move) == 1 && _board.is_black_pawn(sq)) {
                std::vector<char> promotions{'q', 'r', 'b', 'n'};
                for (auto piece : promotions) {
                    Node *spawn = new Node(&_board, sq, move, piece);
                    spawn->parent = this;
                    _child.push_back(spawn);
                    // if (detect_checkmate(&spawn->_board) != 0) { return; }
                }
            }
            else {
                Node *spawn = new Node(&_board, sq, move, 0);
                spawn->parent = this;
                _child.push_back(spawn);
                // if (detect_checkmate(&spawn->_board) != 0) { return; }
            }
        }
    }
    delete _board.maps;
    _board.maps = nullptr;

    for (auto& n : _child) {
        n->spawn(depth - 1);
    }
}

///@warning infinite loop if start node doesn't lead to end node
Node *Node::next_step(Node *end, uint *depth)
{
    Node *current = end;
    while (current->parent != this) {
        current = current->parent;
        (*depth)++;
    }
    return current;
}

void print_nodes(std::vector<Node *> path)
{
    std::reverse(path.begin(), path.end());
    for (const auto& n : path) {
        std::cout << n->_move << " ";
    }
    std::cout << "\n";
}

double discourage_early_queen_movement(const Node *node)
{
    double score = 0;
    if (node->_board.game_state.full_move_number <= 10) {
        if (node->parent != nullptr) {
            if (node->parent->_board.is_queen(node->_from)) {
                if (node->parent->_board.game_state.active_color == Color::white) { score -= .9; }
                if (node->parent->_board.game_state.active_color == Color::black) { score += .9; }
            }
            else if (!node->parent->_board.is_pawn(node->_from)) {
                if (node->parent->_board.game_state.active_color == Color::white) { score += .5; }
                if (node->parent->_board.game_state.active_color == Color::black) { score -= .5; }
            }
        }
    }
    return score;
}

double castle_bonus(Node *n)
{
    double score = 0;
    if (n->parent != nullptr) {
        if (n->parent->_board.is_white_king(n->_from) && n->_from == Square::e1) {
            if (n->_to == Square::g1 || n->_to == Square::c1) { score += CASTLE_BONUS; }
        }
        else if (n->parent->_board.is_black_king(n->_from) && n->_from == Square::e8) {
            if (n->_to == Square::g8 || n->_to == Square::c8) { score -= CASTLE_BONUS; }
        }
    }
    return score;
}

// if it is white's turn, white is going to choose the highest number
// if it is black's turn, black is going to choose the lowest number

#endif //CHESSENGINE_TREE_HPP
