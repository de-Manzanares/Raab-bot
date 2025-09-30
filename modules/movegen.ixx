/**
 * @file movegen.ixx
 * move generators
 */

module;

#include <array>

import board;
import move;
import transposition;
import types;

export module movegen;

//------------------------------------------------------------------------------

export constexpr sz_t ml_sz = 256; ///< move list size

export using MoveList = std::array<Move, ml_sz>;

export using MlIt = std::array<Move, ml_sz>::iterator;

/**
 * populate a container with pseudo-legal moves, return the count
 * @param b the board in question
 * @param out output iterator
 * @return number of pseudo-legal moves
 */
export sz_t movegen(const Board &b, MlIt out);

/**
 * populate a container with pseudo-legal capture moves
 * @param b the board in question
 * @param out
 * @return the number of pseudo-legal capture moves
 * @note used by the quiescence search
 */
export sz_t quiescence_movegen(const Board &b, MlIt out);

/**
 * called after making a move to determine if it is legal (leaves us in check)
 */
export bool is_legal(const Board &b);

/**
 * @param b the board in question
 * @return the count of legal moves
 */
export sz_t cnt_legal_moves(Board &b);

/**
 * find the highest scored move and swap it with `first`
 * if tt_m is not a null move, choose that move swap it with `first`
 * @param first first iterator in range
 * @param sz the number of elements in the range
 * @param tt_m put this move at the front of the move list
 */
export void move_select(MlIt first, sz_t sz, TT_move tt_m = TT_move{});

//------------------------------------------------------------------------------
