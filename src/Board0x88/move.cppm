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
  en_passant_capture,
  castle,
  promotion,
  prom_capture,
  null_flag
};

export struct Move {
  Square from_square = null_square;
  Square to_square = null_square;
  PieceInfo from_piece = {.piece_type = null_piece, .color = null_color};
  Flag flag = null_flag;
  Piece c_piece = null_piece;         ///< captured piece
  Piece promotion_piece = null_piece; ///< promotion piece
  uint8_t ply{};                      ///< ply from root node
  int score{};                        ///< for move ordering
  Square ep_target = null_square;
};

export bool operator==(Move lhs, Move rhs);

/// for tests only, doesn't do full comparison
bool operator==(const Move lhs, const Move rhs) {
  return lhs.from_square == rhs.from_square && lhs.to_square == rhs.to_square &&
         lhs.flag == rhs.flag;
}

// todo move unmove ep target
// todo move unmove promotion

void move(Board &b, const Move m) {
  // move pieces
  b.piece_on[m.from_square] = null_piece;
  b.color_on[m.from_square] = null_color;

  if (m.flag == promotion) {
    b.piece_on[m.to_square] = m.promotion_piece;
    b.color_on[m.to_square] = m.from_piece.color;
  } else {
    b.piece_on[m.to_square] = m.from_piece.piece_type;
    b.color_on[m.to_square] = m.from_piece.color;
  }

  if (m.flag == en_passant_capture) {
    auto sq = m.to_square + (b.stm == white ? S : N);
    b.piece_on[sq] = null_piece;
    b.color_on[sq] = null_color;
  }

  // update king position
  if (m.from_piece.piece_type == king) {
    if (b.stm == white) {
      b.wks = m.to_square;
    } else {
      b.bks = m.to_square;
    }
  }

  // update en_passant target
  if (m.flag == en_passant) {
    b.ep = m.ep_target;
  } else {
    b.ep = null_square;
  }

  // update side to move
  b.stm = ~b.stm;
}

void un_move(Board &b, const Move m) {
  // move pieces
  b.piece_on[m.from_square] = m.from_piece.piece_type;
  b.color_on[m.from_square] = m.from_piece.color;

  if (m.flag == en_passant_capture) {
    auto sq = m.to_square + (b.stm == black ? S : N);
    b.piece_on[sq] = pawn;
    b.color_on[sq] = b.stm;
    b.piece_on[m.to_square] = null_piece;
    b.color_on[m.to_square] = null_color;
  } else {
    if (m.c_piece != null_piece) {
      b.piece_on[m.to_square] = m.c_piece;
      b.color_on[m.to_square] = ~m.from_piece.color;
    } else {
      b.piece_on[m.to_square] = null_piece;
      b.color_on[m.to_square] = null_color;
    }
  }

  // update king position
  if (m.from_piece.piece_type == king) {
    if (~b.stm == white) {
      b.wks = m.from_square;
    } else {
      b.bks = m.from_square;
    }
  }

  // update en_passant target
  if (m.flag == en_passant) {
    b.ep = null_square;
  }

  // update side to move
  b.stm = ~b.stm;
}
