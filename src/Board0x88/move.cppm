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
  std::uint8_t prev_castling_rights{};
};

export bool operator==(Move lhs, Move rhs);

/// for tests only, doesn't do full comparison
bool operator==(const Move lhs, const Move rhs) {
  return lhs.from_square == rhs.from_square && lhs.to_square == rhs.to_square &&
         lhs.flag == rhs.flag;
}

void move(Board &b, const Move m) {
  // move pieces
  b.piece_on[m.from_square] = null_piece;
  b.color_on[m.from_square] = null_color;

  if (m.flag == promotion || m.flag == prom_capture) {
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

  if (m.flag == castle) {
    switch (m.to_square) {
    case c1:
      b.piece_on[a1] = null_piece;
      b.color_on[a1] = null_color;
      b.piece_on[d1] = rook;
      b.color_on[d1] = white;
      b.castling_rights = b.castling_rights & 0b1100;
      break;
    case g1:
      b.piece_on[h1] = null_piece;
      b.color_on[h1] = null_color;
      b.piece_on[f1] = rook;
      b.color_on[f1] = white;
      b.castling_rights = b.castling_rights & 0b1100;
      break;
    case c8:
      b.piece_on[a8] = null_piece;
      b.color_on[a8] = null_color;
      b.piece_on[d8] = rook;
      b.color_on[d8] = black;
      b.castling_rights = b.castling_rights & 0b0011;
      break;
    case g8:
      b.piece_on[h8] = null_piece;
      b.color_on[h8] = null_color;
      b.piece_on[f8] = rook;
      b.color_on[f8] = black;
      b.castling_rights = b.castling_rights & 0b0011;
      break;
    default:
    }
  }

  // update king position
  if (m.from_piece.piece_type == king) {
    if (b.stm == white) {
      b.castling_rights = b.castling_rights & 0b1100;
      b.wks = m.to_square;
    } else {
      b.castling_rights = b.castling_rights & 0b0011;
      b.bks = m.to_square;
    }
  }

  // update castling rights
  if (m.from_piece.piece_type == rook) {
    if (b.castling_rights) {
      switch (m.from_square) {
      case a1:
        b.castling_rights = b.castling_rights & 0b1101;
        break;
      case h1:
        b.castling_rights = b.castling_rights & 0b1110;
        break;
      case a8:
        b.castling_rights = b.castling_rights & 0b0111;
        break;
      case h8:
        b.castling_rights = b.castling_rights & 0b1011;
        break;
      }
    }
  }
  if (m.c_piece == rook) {
    if (b.castling_rights) {
      switch (m.to_square) {
      case a1:
        b.castling_rights = b.castling_rights & 0b1101;
        break;
      case h1:
        b.castling_rights = b.castling_rights & 0b1110;
        break;
      case a8:
        b.castling_rights = b.castling_rights & 0b0111;
        break;
      case h8:
        b.castling_rights = b.castling_rights & 0b1011;
        break;
      }
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
  if (m.flag == castle) {
    switch (m.to_square) {
    case c1:
      b.piece_on[a1] = rook;
      b.color_on[a1] = white;
      b.piece_on[d1] = null_piece;
      b.color_on[d1] = null_color;
      break;
    case g1:
      b.piece_on[h1] = rook;
      b.color_on[h1] = white;
      b.piece_on[f1] = null_piece;
      b.color_on[f1] = null_color;
      break;
    case c8:
      b.piece_on[a8] = rook;
      b.color_on[a8] = black;
      b.piece_on[d8] = null_piece;
      b.color_on[d8] = null_color;
      break;
    case g8:
      b.piece_on[h8] = rook;
      b.color_on[h8] = black;
      b.piece_on[f8] = null_piece;
      b.color_on[f8] = null_color;
      break;
    default:
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

  // update castling rights
  b.castling_rights = m.prev_castling_rights;

  // update side to move
  b.stm = ~b.stm;
}
