/**
 * @file move.cppm
 */

module;

#include "transposition.hpp"

#include <cstdint>
#include <iostream>

export module Board0x88:move;
import :core;
import :fen;

//------------------------------------------------------------------------------

export enum Flag : std::uint8_t {
  normal,
  capture,
  double_push,
  en_passant_capture,
  castle,
  promotion,
  prom_capture,
  null_flag
};

export struct Move {
  Square from_sq = null_square;
  Square to_sq = null_square;
  PieceInfo from_piece = {.piece_t = null_piece, .color = null_color};
  Flag flag = null_flag;
  Piece cap_piece = null_piece; ///< captured piece
  Piece prom_p = null_piece;    ///< promotion piece
  uint8_t ply{};                ///< ply from root node
  int score{};                  ///< for move ordering
  Square ep_target = null_square;
  std::uint8_t prev_cr{};
  Square prev_ep = null_square;

  friend std::ostream &operator<<(std::ostream &os, const Move &m);
};

std::ostream &print_square(std::ostream &os, const Square sq) {
  const char file = static_cast<char>('a' + (sq & 0x7));
  const char rank = static_cast<char>('1' + ((sq >> 4) & 0x7));
  os.put(file).put(rank);
  return os;
}

std::ostream &operator<<(std::ostream &os, const Move &m) {
  if (m.from_sq == null_square) {
    os << "0000";
    return os;
  }
  print_square(os, m.from_sq);
  print_square(os, m.to_sq);
  if (m.prom_p != null_piece) {
    os << fen::get_char_code({m.prom_p, m.from_piece.color});
  }
  return os;
}

export bool operator==(Move lhs, Move rhs);

export void move(Board &b, Move m);
export void unmove(Board &b, Move m);

//------------------------------------------------------------------------------

/// for tests only, doesn't do full comparison
bool operator==(const Move lhs, const Move rhs) {
  return lhs.from_sq == rhs.from_sq && lhs.to_sq == rhs.to_sq &&
         lhs.flag == rhs.flag;
}

void clear_sq(Board &b, const Square sq) {
  if (auto [piece_t, color] = b.piece_info(sq); piece_t != null_piece) {
    b.piece_on[sq] = null_piece;                        // clear
    b.color_on[sq] = null_color;                        // clear
    b.hash ^= zobrist.piece_square[piece_t][color][sq]; // update hash
  } else {
    // no action needed
  }
}

/**
 * @note calls clear square to maintain Zobrist hash
 */
void set_sq(Board &b, const Square sq, const PieceInfo pi) {
  if (is_valid_square(sq)) { // todo when are we passed an invalid square?
    clear_sq(b, sq);
    b.piece_on[sq] = pi.piece_t;
    b.color_on[sq] = pi.color;
    b.hash ^= zobrist.piece_square[pi.piece_t][pi.color][sq];
  }
}

PieceInfo prom_piece(const Move &m) {
  return PieceInfo{m.prom_p, m.from_piece.color};
}

PieceInfo from_piece(const Move &m) {
  return PieceInfo{m.from_piece.piece_t, m.from_piece.color};
}

void rm_castle_rights(Board &b, const Color c) {
  if (c == white) {
    b.cr &= 0b1100;
  } else if (c == black) {
    b.cr &= 0b0011;
  }
}

void rook_mv_castle_rights(Board &b, const Square sq) {
  switch (sq) {
  case a1:
    b.cr &= 0b1101;
    break;
  case h1:
    b.cr &= 0b1110;
    break;
  case a8:
    b.cr &= 0b0111;
    break;
  case h8:
    b.cr &= 0b1011;
  default:
  }
}

PieceInfo captured_piece(const Move &m) {
  return PieceInfo{m.cap_piece, ~m.from_piece.color};
}

void finish_castle(Board &b, const Square to) {
  switch (to) {
  case c1:
    clear_sq(b, a1);
    set_sq(b, d1, {rook, white});
    rm_castle_rights(b, white);
    break;
  case g1:
    clear_sq(b, h1);
    set_sq(b, f1, {rook, white});
    rm_castle_rights(b, white);
    break;
  case c8:
    clear_sq(b, a8);
    set_sq(b, d8, {rook, black});
    rm_castle_rights(b, black);
    break;
  case g8:
    clear_sq(b, h8);
    set_sq(b, f8, {rook, black});
    rm_castle_rights(b, black);
    break;
  default:
  }
}

void unfinish_castle(Board &b, const Square to) {
  switch (to) {
  case c1:
    clear_sq(b, d1);
    set_sq(b, a1, {rook, white});
    break;
  case g1:
    clear_sq(b, f1);
    set_sq(b, h1, {rook, white});
    break;
  case c8:
    clear_sq(b, d8);
    set_sq(b, a8, {rook, black});
    break;
  case g8:
    clear_sq(b, f8);
    set_sq(b, h8, {rook, black});
    break;
  default:
  }
}

export void move(Board &b, Move m);
export void unmove(Board &b, Move m);

void move(Board &b, const Move m) {
  // move pieces
  clear_sq(b, m.from_sq);
  if (m.flag == promotion || m.flag == prom_capture) {
    set_sq(b, m.to_sq, prom_piece(m));
  } else {
    set_sq(b, m.to_sq, from_piece(m));
  }
  if (m.flag == en_passant_capture) {
    const auto sq = m.to_sq + (b.stm == white ? S : N);
    clear_sq(b, sq);
  } else if (m.flag == castle) {
    finish_castle(b, m.to_sq);
  }

  // update king position
  // second condition is to prevent reversal of hash
  if (m.from_piece.piece_t == king && m.flag != castle) {
    if (b.stm == white) {
      b.wks = m.to_sq;
      rm_castle_rights(b, white);
    } else {
      b.bks = m.to_sq;
      rm_castle_rights(b, black);
    }
  }

  // update castling rights
  if (b.cr != 0) {
    if (m.from_piece.piece_t == rook) {
      rook_mv_castle_rights(b, m.from_sq);
    }
    if (m.cap_piece == rook) {
      rook_mv_castle_rights(b, m.to_sq);
    }
  }

  b.hash ^= zobrist.castling[m.prev_cr];
  b.hash ^= zobrist.castling[b.cr];

  if (b.ep != null_square) {
    b.hash ^= zobrist.ep[b.ep]; // undo the previous
  }

  // update en_passant target
  if (m.flag == double_push) {
    b.ep = m.ep_target;
    b.hash ^= zobrist.ep[b.ep];
  } else {
    b.ep = null_square;
  }

  // update side to move
  b.stm = ~b.stm;
  b.hash ^= zobrist.stm;
}

void unmove(Board &b, const Move m) {
  // move pieces
  set_sq(b, m.from_sq, from_piece(m));
  if (m.flag == en_passant_capture) {
    const auto sq = m.to_sq + (b.stm == black ? S : N);
    clear_sq(b, m.to_sq);
    set_sq(b, sq, {pawn, b.stm});
  } else if (m.cap_piece != null_piece) { // cannot be m.flag == capture (?)
    set_sq(b, m.to_sq, captured_piece(m));
  } else {
    clear_sq(b, m.to_sq);
  }
  if (m.flag == castle) {
    unfinish_castle(b, m.to_sq);
  }

  // update king position
  if (m.from_piece.piece_t == king) {
    if (~b.stm == white) {
      b.wks = m.from_sq;
    } else {
      b.bks = m.from_sq;
    }
  }

  // update castling rights
  b.hash ^= zobrist.castling[m.prev_cr];
  b.hash ^= zobrist.castling[b.cr];
  b.cr = m.prev_cr;

  // en passant
  if (b.ep != null_square) {
    b.hash ^= zobrist.ep[b.ep];
  }
  if (m.prev_ep != null_square) {
    b.hash ^= zobrist.ep[m.prev_ep];
  }
  b.ep = m.prev_ep;

  // update side to move
  b.stm = ~b.stm;
  b.hash ^= zobrist.stm;
}
