/**
 * @file move.ixx
 * types and functions associated with moving things on the board
 */

module;

#include <iostream>
#include <vector>

import types;
import board;

export module move;

//------------------------------------------------------------------------------

/// position stack for three-fold repetition detection
std::vector<U64> history;

/// categorize moves
export enum Flag : U8 {
  normal,
  capture,
  double_push,
  en_passant_capture,
  castle,
  promotion,
  prom_capture,
  null_flag
};

/// store all information necessary to make and unmake a move
export struct Move {
  Square    from_sq    = null_square;
  Square    to_sq      = null_square;
  PieceInfo from_piece = {null_piece, null_color};
  Flag      flag       = null_flag;
  Piece     cap_piece  = null_piece; ///< captured piece
  Piece     prom_p     = null_piece; ///< promotion piece
  Square    ep_target  = null_square;
  score_t   score      = 0; ///< for move ordering
  U8        prev_cr    = 0;
  Square    prev_ep    = null_square;
  U8        ply        = 0; ///< ply from root node

  friend std::ostream &operator<<(std::ostream &os, const Move &m);
};

/// for quick is_repetition() execution. doesn't compare full struct
export bool operator==(Move lhs, Move rhs);

/**
 * @param   hash the current board Zobrist hash
 * @return  true - the current position has been seen twice already
 *          false - !true
 */
export bool is_repetition(U64 hash);

/**
 * make the move m
 */
export void move(Board &b, Move m);

/**
 * unmake the move m
 */
export void unmove(Board &b, Move m);

//------------------------------------------------------------------------------
