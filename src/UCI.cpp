#include "../include/UCI.h"

// TODO determine depth using complexity of the board and time left in the game

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
 * @brief Gives info's to stdcout
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
            if (simon_says(&in, "fen")) { }                                     // TODO start from position
            else if (simon_says(&in, "startpos")) {
                n = new Node;
                if (simon_says(&in, "moves")) { startpos_moves(n, &in); }
            }
        }
        else if (simon_says(&in, "go") && n != nullptr) {
            // TODO memory is better, but smaller still is needed
            // move gen is too slow. 4ply is asking > 1M nodes in early middle-game positions
            uint D = 3;
            Counter::node = 0;

            bool maxing = is_maxing(n);
            n->_board->update_move_maps();
            std::thread status_thread(status_update_thread, 1000);
            n->spawn_depth_first(D);

            continue_status_updates = false;
            status_thread.join();       // TODO reduce sleep so that join can occur sooner

            // n->spawn_breadth_first(D);

            std::vector<Node *> opt_nodes{Search::min_max(n, D, neg_inf, pos_inf, maxing)};
            uint depth_counter = 1;
            std::vector<Node *> moves{(n->next_step(opt_nodes[0], &depth_counter))};

            auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::high_resolution_clock::now() - Counter::start).count();

            std::cout << "info"
                      << " depth " << opt_nodes[0]->node_depth()
                      << " time " << time
                      << " nodes " << Counter::node
                      << "\nbestmove "
                      << long_algebraic_notation(moves[0])
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
    if (*in == "uci") { std::cout << "id name Raab-bot\nid author Schauss\nuciok\n"; }

        // to give the engine time to set up stuff ... but we don't have any stuff!!!
    else if (*in == "isready") { std::cout << "readyok\n"; }
}

/**
 * @brief Performs moves on a board as dictated by a string of long algebraic notation
 * @param n Pointer to a Node holding the board to be mutated.
 * @param in Pointer to a string containing the
 */
void startpos_moves(Node *n, std::string *in)
{
    std::istringstream iss(*in);
    std::string s;

    // get rid of "position startpos moves" so we can process the moves
    for (auto i = 0; i < 3; i++) { iss >> s; }

    // the following "words" in the string will be moves
    while (iss >> s) {
        Square from, to;
        char ch;
        string_to_move(&s, &from, &to, &ch);
        n->_board->move(from, to, ch);
    }
}
