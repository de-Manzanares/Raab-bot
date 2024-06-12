#ifndef CHESSENGINE_SEARCH_H
#define CHESSENGINE_SEARCH_H

#include "Board.h"
#include "Eval.h"
#include "Node.h"

struct Search {
    static Node *min_max(Node *n, uint depth, double alpha, double beta, bool maximizing);
};

#endif //CHESSENGINE_SEARCH_H
