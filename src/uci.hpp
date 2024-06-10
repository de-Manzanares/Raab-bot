#ifndef CHESSENGINE_UCI_HPP
#define CHESSENGINE_UCI_HPP

#include <iostream>
#include "tree.hpp"
#include "search.hpp"

void string_to_move(const std::string *string, Square *from, Square *to, char *ch);
void preamble(const std::string *in);
void startpos_moves(Node *n, std::string *in);

bool is_maxing(Node *n) { return n->_board.game_state.active_color == Color::white; }

// PARAMS
const bool INFO = true;
double neg_inf = -100'000;
double pos_inf = 100'000;

/**
 * @class uci
 * @brief Implements the UCI (Universal Chess Interface) protocol.
 */
struct uci {
    static void loop();
};

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
        if (in.find("position") != std::string::npos) {

            // TODO import fen, should be as easy as just grabbing the rest of the line
            if (in.find("fen") != std::string::npos) { }

            else if (in.find("startpos") != std::string::npos) {
                n = new Node;   // start a new tree TODO is the import necessary?
                n->_board.import_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

                // if there are moves to make, make them
                if (in.find("moves") != std::string::npos) { startpos_moves(n, &in); }
            }
        }

        if (in.find("go") != std::string::npos) {
            //start calculating for current position
            // TODO determine depth using complexity of the board and time left in the game
            // if there is plenty of time, dig deeper
            n->_board.update_move_maps();
            uint D;
            uint pieces = 0;
            for (const auto& [sq, moves] : n->_board.maps->move_map_white) {
                if (!moves.empty()) { pieces++; }
            }
            for (const auto& [sq, moves] : n->_board.maps->move_map_black) {
                if (!moves.empty()) { pieces++; }
            }
            D = 3;

            if (INFO) { std::cout << "info branch depth " << D << "\n"; }
            if (INFO) { std::cout << "info moving pieces on the board: " << pieces << "\n"; }

            n->spawn(D);
            std::vector<Node *> opt_nodes{min_max(n, D, neg_inf, pos_inf, is_maxing(n))};
            uint depth_counter = 1;
            std::vector<Node *> moves{(n->next_step(opt_nodes[0], &depth_counter))};

            if (INFO) { std::cout << "info depth " << depth_counter << "\n"; }

            std::cout << "bestmove " << moves[0]->_move << "\n";
            delete n;
        }
        if (in.find("stop") != std::string::npos) { }   // TODO stop calculating
        else if (in == "quit") { break; }               // quit the loop, ends the program
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
            *from = string_to_square(temp);
            temp.clear();
        }
        else if (count == 4) {
            *to = string_to_square(temp);
            temp.clear();
        }
        else if (count == 5) {
            *ch = c;
        }
    }
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
        n->_board.move(from, to, ch);
    }
}

#endif //CHESSENGINE_UCI_HPP
