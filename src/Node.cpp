#include "../include/Node.h"

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


// initialize static counter variables
uint Counter::node = 0;
std::chrono::time_point<std::chrono::high_resolution_clock> Counter::start = std::chrono::high_resolution_clock::now();

/**
 * @brief Default constructor for Node class
 */
Node::Node()
        :_board(std::make_shared<Board>())
{
    _board->update_move_maps();
    _eval = Eval::eval(_board) + Eval::discourage_early_queen_movement(this) + Eval::castle_bonus(this);
}

/**
 * @brief Create a Node with the given FEN string.
 * @param fen The FEN string representing a game state
 */
Node::Node(const std::string& fen)
        :_board(std::make_shared<Board>())
{
    _board->import_fen(fen);
    _board->update_move_maps();
    _eval = Eval::eval(_board) + Eval::discourage_early_queen_movement(this) + Eval::castle_bonus(this);
}

/**
 * @brief Constructor used in tree generation
 * @param board The board to be copied.
 * @param from The starting square of the move to be made.
 * @param to The ending square of the move to be made.
 * @param ch The character representation of the piece to promote the pawn to, if the move is a pawn promotion.
 */
Node::Node(const std::shared_ptr<Board>& board, Square from, Square to, char ch)
        :_board(std::make_shared<Board>())
{
    *_board = *board;
    _from = from;
    _to = to;
    _board->move(from, to, ch);
    _move += Sq::square_to_string(from) += Sq::square_to_string(to);
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
        _board->update_move_maps();
        _eval = Eval::eval(_board);
        delete _board->maps;
        _board->maps = nullptr;
        return;
    }

    _board->update_move_maps();
    _eval = Eval::eval(_board);

    if (Eval::detect_checkmate(_board) != 0) { return; }

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

    // delete _board->maps;
    // _board->maps = nullptr;
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
