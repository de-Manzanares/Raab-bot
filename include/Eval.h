#ifndef INCLUDE_EVAL_H_
#define INCLUDE_EVAL_H_

#include <memory>
#include <unordered_map>

#include "Board.h"
#include "Node.h"

struct Node;

struct Eval {
    static std::unordered_map<char, int> material_value;

    // evaluation parameters
    static double CHECK_BONUS;
    static double MOBILITY_MULTIPLIER;
    static double CASTLE_BONUS;
    static double RATIO_MULTIPLIER;

    // evaluation functions ----------------------------------------

    static double material_evaluation(const Node *n);
    static int detect_checkmate(const Node *n);
    static double mobility_evaluation(const Node *n);
    static double check_bonus(const Node *n);
    static double castle_bonus(const Node *n);
    static double simple_evaluation(const Node *n);
    static double eval(const Node *n);

    // unused
    [[maybe_unused]] static double material_ratio(const Board *board);
    [[maybe_unused]] static double discourage_early_queen_movement(const Node *node);
};

#endif  // INCLUDE_EVAL_H_
