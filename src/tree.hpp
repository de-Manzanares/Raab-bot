#ifndef CHESSENGINE_TREE_HPP
#define CHESSENGINE_TREE_HPP

#include <limits>
#include <queue>
#include <thread>
#include "Board.hpp"

// TODO add node count, nps, time controls on tree gen

// TODO iterative deepening
// TODO give weight to controlling opposing king's squares
// spawn based on time left and complexity (predicted nodes)
// TODO give weight to stalemate
// TODO passed pawns good
// TODO stacked pawns bad
// TODO avoid pawn forks LOL
// TODO move order
// TODO influencing the center
// TODO include influence as well as legal moves (instead of ?)
// TODO enable time control on analysis


//----------------------------------------------------------------------------------------------------------------------
// BEGIN EVALUATION

struct Node;
double discourage_early_queen_movement(const Node *node);
double castle_bonus(Node *n);

const double CHECK_BONUS = 0.25;                     // gives weight to checks 0.25
const double MOBILITY_MULTIPLIER = 1.0 / 100.0;     // gives weight for each legal move 1.0
const double CASTLE_BONUS = 0.5;                    // gives weight to castling 0.5
const double RATIO_MULTIPLIER = 2.0;                // gives weight to ratio of material advantage

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
 * @brief Calculates the material ratio for a given chess board.
 * @details intended to encourage trading when up on material, and discourage trading when down on material
 * @param board Pointer to the Board object representing the chess board.
 * @return The ratio of material advantage to total material. Pos for white neg for black.
 * indicates an advantage for black.
 */
double material_ratio(const Board *board)
{
    double wscore = 0;
    double bscore = 0;
    double ratio = 0;
    for (Square sq = Square::a8; sq >= Square::h1; --sq) {
        if (!board->is_empty(sq)) {
            if (board->is_white(sq)) { wscore += material_value[board->what_piece(sq)]; }
            else if (board->is_black(sq)) { bscore += material_value[board->what_piece(sq)]; }
        }
    }
    if (wscore == -bscore) { return 0; }
    return RATIO_MULTIPLIER * ((wscore + bscore) / (wscore - bscore));
}

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
        if (board->game_state.in_check_white) {
            for (const auto& [sq, moves] : board->maps->move_map_white) { number_of_moves += moves.size(); }
            if (number_of_moves == 0) { return -1; }
        }
    }
    // detect black checkmate
    if (board->game_state.active_color == Color::black) {
        if (board->game_state.in_check_black) {
            for (const auto& [sq, moves] : board->maps->move_map_black) { number_of_moves += moves.size(); }
            if (number_of_moves == 0) { return 1; }
        }
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

/**
 * Creates an aggregate score using all evaluation methods
 */
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

/**
 * wrapper for simple_evaluation()
 */
double eval(const Board *board)
{
    return simple_evaluation(board);
}

// END EVALUATION
//----------------------------------------------------------------------------------------------------------------------
// BEGIN NODE

// TODO make nodes as small as possible
// TODO get rid of move, redundant with _from and _to

struct Counter {
    static uint node;
    static std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

uint Counter::node = 0;
std::chrono::time_point<std::chrono::high_resolution_clock> Counter::start = std::chrono::high_resolution_clock::now();

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

/**
 * @brief Default constructor for Node class
 */
Node::Node()
{
    _board.update_move_maps();
    _eval = eval(&_board) + discourage_early_queen_movement(this) + castle_bonus(this);
}

/**
 * @brief Create a Node with the given FEN string.
 * @param fen The FEN string representing a game state
 */
Node::Node(const std::string& fen)
{
    _board.import_fen(fen);
    _board.update_move_maps();
    _eval = eval(&_board) + discourage_early_queen_movement(this) + castle_bonus(this);
}

/**
 * @brief Constructor used in tree generation
 * @param board The board to be copied.
 * @param from The starting square of the move to be made.
 * @param to The ending square of the move to be made.
 * @param ch The character representation of the piece to promote the pawn to, if the move is a pawn promotion.
 */
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

/**
 * @brief Destructor for the Node class
 */
Node::~Node()
{
    for (auto& child : _child) {
        delete child;
        child = nullptr;
    }
}

/**
 * @brief Counts the total number of nodes in the tree starting from the node calling the function.
 * @return The total number of nodes in the tree "below" the calling node.
 */
uint Node::count_nodes()
{
    uint count = _child.size();
    for (const auto& child : _child) { count += child->count_nodes(); }
    return count;
}

/**
 * @brief Creates a decision tree of n layers.
 * @param depth The depth of the tree to spawn child nodes for.
 */
void Node::spawn_depth_first(uint depth)
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
                    Counter::node++;
                }
            }
            else {
                Node *spawn = new Node(&_board, sq, move, 0);
                spawn->parent = this;
                _child.push_back(spawn);
                Counter::node++;
            }
        }
    }

    // TODO partial tree gen is only good for breadth first?
    // auto now = std::chrono::high_resolution_clock::now();
    // auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - Counter::start).count();
    // if (elapsed_ms > 15000) { return; }

    delete _board.maps;
    _board.maps = nullptr;
    for (auto& n : _child) {
        n->spawn_depth_first(depth - 1);
    }
}

/**
 * @brief Finds the next step from the current node to the end node
 * @param end The end node to find the next step to
 * @param depth Pointer to the depth variable, which will be updated during traversal
 * @return The next step node from the current node to the end node
 * @warning The function will result in an infinite loop if the current node is not a parent of the end node
 */
Node *Node::next_step(Node *end, uint *depth)
{
    Node *current = end;
    while (current->parent != this) {
        current = current->parent;
        (*depth)++;
    }
    return current;
}

uint Node::node_depth()
{
    uint ply = 0;
    Node *current = this;
    while (current->parent != nullptr) {
        current = current->parent;
        ply++;
    }
    return ply;
}
// END NODE
//---------------------------------------------------------------------------------------------------------------------

/**
 * In the first ten moves of the game, penalize queen movements, encourage other piece movements
 * @note is not needed when an opening book is in use
 */
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

/**
 * Encourage castling lol
 * I just wanted to see castling more often so I tacked some weight onto it.
 */
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

#endif //CHESSENGINE_TREE_HPP
