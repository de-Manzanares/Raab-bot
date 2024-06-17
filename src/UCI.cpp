#include "../include/UCI.h"

// TODO ponder ... HOW?!

/**
 * @return True - white to move \n False - black to move
 */
bool is_maxing(Node *n) { return n->_board->game_state.active_color == Color::white; }

/**
 * @brief Does given string contains another string.
 * @param s A pointer to the string being searched.
 * @param has The string to search for.
 * @return True - the substring is found \n False - the substring is not found
 */
bool simon_says(const std::string *s, const std::string& has) { return s->find(has) != std::string::npos; }

// parameters
double neg_inf = -100'000;
double pos_inf = 100'000;

bool continue_status_updates = true;

/**
 * @brief Gives info's to std out
 * @param update_interval_ms The update interval in milliseconds.
 */
void status_update_thread(uint update_interval_ms)
{
    uint previous = 0;
    Counter::start = std::chrono::high_resolution_clock::now();

    while (continue_status_updates) {
        uint current = Counter::node;
        auto now = std::chrono::high_resolution_clock::now();

        std::cout << "info"
                  // << " depth "
                  << " time " << std::chrono::duration_cast<std::chrono::milliseconds>(now - Counter::start).count()
                  << " nodes " << Counter::node
                  << " nps " << current - previous
                  << std::endl;     // GOTTA FLUSH THE BUFFER!!!
        std::this_thread::sleep_for(std::chrono::milliseconds(update_interval_ms));

        previous = current;
    }
}

/**
 * @brief Implements the main loop that listens to UCI commands and processes them.
 */
void uci::loop()
{
    std::string in;     // the command from the GUI
    Node *n;            // root node is a pointer for easy deletion and rebuilding of decision tree

    while (std::getline(std::cin, in)) {
        preamble(&in);

        // set up a board in the given position, wait for "go" to search, a fen or a list of moves may follow
        if (simon_says(&in, "position")) {
            if (simon_says(&in, "fen")) {
                std::string fen_tag = "position fen ";
                std::string moves_tag = " moves ";

                size_t start_index = in.find(fen_tag);
                size_t end_index = in.find(moves_tag);

                std::string s_fen;
                std::string s_moves;

                if (start_index != std::string::npos) {
                    if (end_index != std::string::npos) {
                        s_fen = in.substr(start_index + fen_tag.length(), end_index - start_index - fen_tag.length());
                        s_moves = in.substr(end_index + moves_tag.length());
                    }
                    else { s_fen = in.substr(start_index + fen_tag.length()); }
                }

                n = new Node(s_fen);
                if (!s_moves.empty()) { moves(n, &s_moves); }
            }
            else if (simon_says(&in, "startpos")) {
                n = new Node;
                if (simon_says(&in, "moves")) {
                    in = in.substr(std::string("position startpos moves ").length());
                    moves(n, &in);
                }
            }
        }
        else if (simon_says(&in, "go") && n != nullptr) {
            // move gen is too slow. 4ply is asking > 1M nodes in early middle-game positions
            uint D = 3;                                                 // set search depth
            Counter::node = 0;                                          // reset node counter
            bool maxing = is_maxing(n);                                 // for minmax search
            n->_board->update_move_maps();                              // for move generation
            std::thread status_thread(status_update_thread, 1000);      // start timer
            n->spawn_depth_first(D);                                    // generate tree
            continue_status_updates = false;                            // end timer
            status_thread.join();

            // get the optimal node, and get the next steps on the path to the optimal node
            // 1st step - best move, 2nd step - ponder
            std::vector<Node *> opt_node{Search::min_max(n, D, neg_inf, pos_inf, maxing)};
            std::vector<Node *> path{(n->next_step(opt_node[0]))};      // first step
            path.push_back(path[0]->next_step(opt_node[0]));            // second step

            auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::high_resolution_clock::now() - Counter::start).count();

            std::cout << "info"
                      << " depth " << opt_node[0]->node_depth()
                      << " time " << time
                      << " nodes " << Counter::node
                      << "\n"
                      << "bestmove " << long_algebraic_notation(path[0])
                      << " ponder " << long_algebraic_notation(path[1])
                      << std::endl;

            delete n;
            n = nullptr;
        }
        else if (in.find("stop") != std::string::npos) { }      // TODO stop calculating
        else if (in == "quit") { break; }                       // quit the loop, ends the program
    }
}

/**
 * @brief Decomposes long algebraic notation into a format this program can process.
 * @param string The string representation of the chess move.
 * @param from Pointer to the Square variable to store the source square.
 * @param to Pointer to the Square variable to store the destination square.
 * @param ch Pointer to the char variable to store the additional character.
 */
void string_to_move(const std::string *string, Square *from, Square *to, char *ch)
{
    std::string temp;
    auto count = 0;
    for (const auto c : *string) {
        if (c != 'x' && c != '#') {
            temp.push_back(c);
            count++;
        }
        if (count == 2) {
            *from = Sq::string_to_square(temp);
            temp.clear();
        }
        else if (count == 4) {
            *to = Sq::string_to_square(temp);
            temp.clear();
        }
        else if (count == 5) {
            *ch = c;
        }
    }
}

/**
 * @param n The node to convert.
 * @return long algebraic notation of a node's move.
 */
std::string long_algebraic_notation(const Node *n)
{
    std::string s;
    s += Sq::square_to_string(n->_from) += Sq::square_to_string(n->_to);
    if (n->_ch != 0) { s += n->_ch; }
    return s;
}

/**
 * @brief Handles the preamble for the engine in UCI protocol
 * @param in A pointer to a string containing the input command
 */
void preamble(const std::string *in)
{
    // should give engine options to be configured ... we don't have any right now, so ... uciok!
    if (*in == "uci") {
        std::cout << "id name Raab-bot\n"
                     "id author Schauss\n\n"
                     "option name Ponder type check default true\n"
                     "uciok\n";
    }

        // to give the engine time to set up stuff ... but we don't have any stuff!!!
    else if (*in == "isready") { std::cout << "readyok\n"; }
}

/**
 * @brief Performs moves on a board as dictated by a string of long algebraic notation
 * @param n Pointer to a Node holding the board to be mutated.
 * @param in Pointer to a string containing the
 */
void moves(Node *n, std::string *in)
{
    std::istringstream iss(*in);
    std::string s;

    // the following "words" in the string will be moves
    while (iss >> s) {
        Square from, to;
        char ch;
        string_to_move(&s, &from, &to, &ch);
        n->_board->move(from, to, ch);
    }
}
