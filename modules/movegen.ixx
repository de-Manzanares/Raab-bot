/**
 * @file movegen.ixx
 * move generators
 */

module;

#include <array>
#include <span>

import board;
import defs;
import move;
import transposition;

export module movegen;

namespace raab_bot {

//------------------------------------------------------------------------------

export constexpr sz_t ml_sz = 256; ///< move list size

export using MoveList = std::array<Move, ml_sz>;

export using MlIt = std::array<Move, ml_sz>::iterator;

/**
 * populate a container with pseudo-legal moves
 * @param b the board in question
 * @param out output iterator
 * @return a view of the generated moves
 */
export std::span<Move> movegen(const Board &b, MlIt out);

/**
 * populate a container with pseudo-legal capture moves
 * @param b the board in question
 * @param out output iterator
 * @return a view of the generated moves
 * @note used by the quiescence search
 */
export std::span<Move> quiescence_movegen(const Board &b, MlIt out);

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
 * find the highest scored move and swap it with the front element
 * if tt_m is not a null move, place that move at the front
 * @param ml a span of moves
 * @param tt_m (optional) put this move at the front of the move list
 */
export void move_select(std::span<Move> ml, TT_move tt_m = TT_move{});

//------------------------------------------------------------------------------

} // namespace raab_bot
