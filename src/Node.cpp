#include "../include/Node.h"

// TODO add time controls on tree gen
// TODO ponder (will help on predictable exchanges at least)
// TODO timing profiles - depth based on time controls
// spawn based on time left and complexity (predicted nodes)
// TODO iterative deepening
// TODO give weight to controlling opposing king's squares
// TODO give weight to stalemate
// TODO passed pawns good
// TODO stacked pawns bad
// TODO avoid pawn forks LOL
// TODO move order
// TODO influencing the center
// TODO include influence as well as legal moves (instead of ?) would allow for attack/defender counting
// TODO enable time control on analysis

// initialize static counter variables
uint Counter::node = 0;
std::chrono::time_point<std::chrono::high_resolution_clock> Counter::start = std::chrono::high_resolution_clock::now();

/**
 * @brief Default constructor for Node class
 */
Node::Node() :_board(std::make_shared<Board>())
{
    _from = _to = Square::h1;                               // TODO make null squares?
    _ch = 0;
}

/**
 * @brief Create a Node with the given FEN string.
 * @param fen The FEN string representing a game state
 */
Node::Node(const std::string& fen) :_board(std::make_shared<Board>())
{
    _board->import_fen(fen);
    _from = _to = Square::h1;
    _ch = 0;
}

/**
 * @brief Constructor used in tree generation
 * @param board The board to be copied.
 * @param from The starting square of the move to be made.
 * @param to The ending square of the move to be made.
 * @param ch The character representation of the piece to promote the pawn to, if the move is a pawn promotion.
 */
Node::Node(const std::shared_ptr<Board>& board, Square from, Square to, char ch) :_board(std::make_shared<Board>())
{
    *_board = *board;
    _from = from;
    _to = to;
    _ch = ch;
    _board->move(from, to, ch);
}

/**
 * @brief Destructor for the Node class
 */
Node::~Node() { for (auto& child : _child) { delete child; }}

/**
 * @brief Counts the total number of nodes in the tree starting from the node calling the function.
 * @return The total number of nodes in the tree "below" the calling node.
 */
uint Node::count_nodes()                            // NOLINT(misc-no-recursion)
{
    uint count = _child.size();
    for (const auto& child : _child) { count += child->count_nodes(); }
    return count;
}

// TODO clean up spawn_depth_first
/**
 * @brief Creates a decision tree of n layers.
 * @param depth The depth of the tree to spawn child nodes for.
 */
void Node::spawn_depth_first(uint depth)            // NOLINT(misc-no-recursion)
{
    if (depth == 0) {                               // terminal nodes
        _board->update_move_maps();
        _eval = Eval::eval(this);
        _board.reset();
        return;
    }

    _board->update_move_maps();

    if (Eval::detect_checkmate(this) != 0) {      // not at specified depth, but still a terminal node
        _eval = Eval::eval(this);
        _board.reset();
        return;
    }

    for (const auto& [sq, moves] :
            _board->game_state.active_color == Color::white ?
            _board->maps->move_map_white : _board->maps->move_map_black) {
        for (const auto& move : moves) {
            if ((_board->is_in_row(move) == 8 && _board->is_white_pawn(sq))
                    || _board->is_in_row(move) == 1 && _board->is_black_pawn(sq)) {
                std::vector<char> promotions{'q', 'r', 'b', 'n'};
                for (auto piece : promotions) {
                    Node *spawn = new Node(_board, sq, move, piece);
                    spawn->parent = this;
                    _child.push_back(spawn);
                    Counter::node++;
                }
            }
            else {
                Node *spawn = new Node(_board, sq, move, 0);
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

    _board.reset();

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

/**
 * @return The distance in ply from a this node to the root node.
 */
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
