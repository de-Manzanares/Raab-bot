/**
 * @file move.cppm
 */

module;

#include <cstdint>

export module Board0x88:move;
import :core;

export enum Flag : std::uint8_t {
  normal,
  capture,
  en_passant,
  castle,
  promotion,
  prom_capture,
  null_flag
};

export struct Move {
  Square from = null_square;
  Square to = null_square;
  Flag flag = null_flag;
  Piece c_piece = null_piece; ///< captured piece
  Piece p_piece = null_piece; ///< promotion piece
};

export bool operator==(Move lhs, Move rhs);

bool operator==(const Move lhs, const Move rhs) {
  return lhs.from == rhs.from && lhs.to == rhs.to && lhs.flag == rhs.flag &&
         lhs.c_piece == rhs.c_piece && lhs.p_piece == rhs.p_piece;
}

void move(Board &b, const Move m) {
  PieceInfo pi = b.piece_info(m.from);

  b.piece_on[m.from] = null_piece;
  b.color_on[m.from] = null_color;

  b.piece_on[m.to] = pi.piece_type;
  b.color_on[m.to] = pi.color;

  b.stm = ~b.stm;
}

void un_move(Board &b, Move m) {
  PieceInfo pi = b.piece_info(m.from);

  b.piece_on[m.from] = pi.piece_type;
  b.color_on[m.from] = pi.color;

  if (m.c_piece != null_piece) {
    b.piece_on[m.to] = m.c_piece;
    b.color_on[m.to] = ~pi.color;
  }

  b.stm = ~b.stm;
}
