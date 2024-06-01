#include <iostream>
#include "Board.hpp"

// TODO just get it working first...you can go back later and make it more efficient =-)
// don't get to worried about writing clever code =-)
// keep it simple

int main()
{
    std::string in;

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
            // todo board constructor using fen
            Board board;
            if (in.find("startpos") != std::string::npos) {
                // board is in startpos by default
            }
            if (in.find("moves") != std::string::npos) {
                // get moves from the string and perform them
            }
        }

        if (in.find("go") != std::string::npos) {
            // start calculating for current position
        }
        if (in.find("stop") != std::string::npos) {
            // stop calculating
        }

        else if (in == "quit") { break; }
    }
    return 0;
}
