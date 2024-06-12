#ifndef CHESSENGINE_UCI_H
#define CHESSENGINE_UCI_H

#include "Board.h"
#include "Eval.h"
#include "Node.h"
#include "Search.h"

#include <thread>

void string_to_move(const std::string *string, Square *from, Square *to, char *ch);
void preamble(const std::string *in);
void startpos_moves(Node *n, std::string *in);
bool is_maxing(Node *n);
bool simon_says(const std::string *s, const std::string& has);
void status_update_thread(uint update_interval_ms);

/**
 * @class uci
 * @brief Implements the UCI (Universal Chess Interface) protocol.
 */
struct uci {
    static void loop();
};

#endif //CHESSENGINE_UCI_H
