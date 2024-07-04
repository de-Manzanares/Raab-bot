/*
 *     ____              __          __          __
 *    / __ \____ _____ _/ /_        / /_  ____  / /_
 *   / /_/ / __ `/ __ `/ __ \______/ __ \/ __ \/ __/
 *  / _, _/ /_/ / /_/ / /_/ /_____/ /_/ / /_/ / /_
 * /_/ |_|\__,_/\__,_/_.___/     /_.___/\____/\__/
 *
 * Copyright (c) 2024 de-Manzanares
 * This work is released under the MIT license.
 *
 */

#ifndef INCLUDE_UCI_H_
#define INCLUDE_UCI_H_

#include "Board.h"
#include "Node.h"

#include <memory>
#include <string>
#include <thread>

namespace uciloop {

/**
 * @brief Does string s contain string has
 * @param s The container
 * @param has The value
 * @return True - the substring is found \n False - the substring is not found
 */
bool simon_says(const std::string *s, const std::string &has);

/// @return True - white to move \n False - black to move
bool is_maxing(const std::shared_ptr<Node> &n);

/**
 * @brief Gives info's to std out
 * @param update_interval_ms The update interval in milliseconds.
 */
void status_update_thread(uint update_interval_ms);

/**
 * @brief Handles the preamble for the engine in UCI protocol
 * @param in A pointer to a string containing the input command
 */
void preamble(const std::string *in);

/**
 * @brief Performs moves from UCI
 * @param n Node holding the board to be mutated.
 * @param in moves string
 */
void startpos_moves(const std::shared_ptr<Node> &n, const std::string *in);

/**
 * @brief Decomposes long algebraic notation
 * @param string The string representation of the chess move.
 * @param from The starting square of the move
 * @param to The target square of the move
 * @param ch If we are promoting a pown, this is the piece to promote to.
 */
void string_to_move(const std::string *string, Square *from, Square *to,
                    char *ch);

/**
 * @param n The node to convert.
 * @return long algebraic notation of a node's move.
 * @note a node's "move" is the move performed that makes this node different
 * from it's parent node
 */
std::string long_algebraic_notation(const std::shared_ptr<Node> &n);

} // namespace uciloop

/**
 * @class uci
 * @brief Handles the UCI (Universal Chess Interface) protocol.
 */
struct uci {
  /// @brief The main loop that listens for and processes UCI commands
  static void loop();
  static uint DEPTH; ///< predefined target depth
};

#endif // INCLUDE_UCI_H_
