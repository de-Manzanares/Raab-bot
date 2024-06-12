#ifndef INCLUDE_UCI_H_
#define INCLUDE_UCI_H_

#include "Board.h"
#include "Eval.h"
#include "Node.h"
#include "Search.h"

#include <string>
#include <thread>

void preamble(const std::string *in);
void startpos_moves(Node *n, std::string *in);
void string_to_move(const std::string *string, Square *from, Square *to, char *ch);
bool simon_says(const std::string *s, const std::string& has);
bool is_maxing(Node *n);
void status_update_thread(uint update_interval_ms);

/**
 * @class uci
 * @brief Handles the UCI (Universal Chess Interface) protocol.
 */
struct uci {
    static void loop();
};

#endif  // INCLUDE_UCI_H_
