#ifndef INCLUDE_SEARCH_H_
#define INCLUDE_SEARCH_H_

#include "Board.h"
#include "Eval.h"
#include "Node.h"

struct Search {
    static Node *min_max(Node *n, uint depth, double alpha, double beta, bool maximizing);
};

#endif  // INCLUDE_SEARCH_H_
