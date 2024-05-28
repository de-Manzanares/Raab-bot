#ifndef CHESSENGINE_MOVE_AND_RULES_HPP
#define CHESSENGINE_MOVE_AND_RULES_HPP

// start with e2e4
// position startpos moves e2e4
// we are going to gives this function "e2e4"

#include "Board.hpp"

uint square_to_uint(std::string square)
{
    uint row = square[1] - '1';
    uint column = 8 - (square[0] - 'a');
    return 8 * row + column - 1;
}

// directional pieces
int forward(int x) { return x * 8; }

int backward(int x) { return -(x * 8); }

int left(int x) { return x; }

int right(int x) { return -x; }

// let's get some move sets down!
struct Move_Set {
    struct Pawn {
    };
};

#endif //CHESSENGINE_MOVE_AND_RULES_HPP
