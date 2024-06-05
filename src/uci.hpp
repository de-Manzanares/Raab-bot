#ifndef CHESSENGINE_UCI_HPP
#define CHESSENGINE_UCI_HPP

#include <iostream>
#include "tree.hpp"
#include "search.hpp"

void string_to_move(const std::string *string, Square *from, Square *to, char *ch);

uint D = 2;   // depth
double neg_inf = -std::numeric_limits<double>::infinity();
double pos_inf = std::numeric_limits<double>::infinity();

bool is_maxing(Node *n)
{
    return n->_board.game_state.active_color == Color::white;
}

struct uci {
    static void loop();
};

void uci::loop()
{
    std::string in;
    Node *n;

    while (std::getline(std::cin, in)) {

        if (in == "uci") {
            // should give engine options to be configured ... we don't have any right now, so ... uciok!
            std::cout << "id name Raab\n"
                      << "id author Schauss\n"
                      << "uciok\n";
        }
        else if (in == "isready") {
            // to give the engine time to set up stuff ... but we don't have any stuff!!!
            std::cout << "readyok\n";
        }

        if (in.find("position") != std::string::npos) {
            // set up a board in the given position, wait for "go" to search
            // a fen or a list of moves may follow "position"
            if (in.find("fen") != std::string::npos) {
                // import fen
            }
            if (in.find("startpos") != std::string::npos) {
                n = new Node;
                n->_board.import_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
                if (in.find("moves") != std::string::npos) {
                    // get moves from the string and perform them
                    std::istringstream iss(in);
                    std::string s;
                    // get rid of "position startpos moves"
                    for (auto i = 0; i < 3; i++) {
                        iss >> s;
                    }
                    // next string will be a move
                    while (iss >> s) {
                        Square from, to;
                        char ch;
                        string_to_move(&s, &from, &to, &ch);
                        n->_board.move(from, to, ch);
                    }
                    // std::cout << "moves found" << "\n";
                }
            }
        }
        if (in.find("go") != std::string::npos) {
            //start calculating for current position
            n->spawn(D);
            std::vector<Node *> opt_nodes{min_max(n, D, neg_inf, pos_inf, is_maxing(n))};
            opt_nodes[0]->spawn(D);
            opt_nodes.push_back(min_max(n, D, neg_inf, pos_inf, is_maxing(n)));
            std::vector<Node *> moves{(n->next_step(opt_nodes[1]))};
            std::cout << "bestmove " << moves[0]->_move << "\n";
            delete n;
        }
        if (in.find("stop") != std::string::npos) {
            // stop calculating
        }

        else if (in == "quit") { break; }
    }

}

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

#endif //CHESSENGINE_UCI_HPP
