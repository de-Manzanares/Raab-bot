#ifndef INCLUDE_EVAL_H_
#define INCLUDE_EVAL_H_

#include "Board.h"
#include "Node.h"

struct Node;

struct Eval {

    static std::unordered_map<char, int> material_value;

    static double CHECK_BONUS;
    static double MOBILITY_MULTIPLIER;
    static double CASTLE_BONUS;
    static double RATIO_MULTIPLIER;

    static double discourage_early_queen_movement(const Node *node);
    static double castle_bonus(Node *n);

    static double material_ratio(const Board *board);
    static double material_evaluation(const Board *board);
    static int detect_checkmate(const Board *board);
    static double mobility_evaluation(const Board *board);
    static double check_bonus(const Board *board);
    static double simple_evaluation(const Board *board);
    static double eval(const Board *board);
};

#endif // INCLUDE_EVAL_H_