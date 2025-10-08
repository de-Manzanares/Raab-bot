module;

#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>

import board;
import defs;
import eval;
import fen;
import transposition;

module move;

namespace raab_bot {

PieceInfo captured_piece(const Move &m);
PieceInfo from_piece(const Move &m);
PieceInfo prom_piece(const Move &m);

void clear_sq(Board &b, Square sq);
void set_sq(Board &b, Square sq, PieceInfo pi);
void finish_castle(Board &b, Square to);
void unfinish_castle(Board &b, Square to);
void rm_castle_rights(Board &b, Color c);
void rook_mv_castle_rights(Board &b, Square sq);

std::ostream &print_square(std::ostream &os, Square sq);

//------------------------------------------------------------------------------

bool operator==(const Move &lhs, const Move &rhs)
{
  return lhs.from_sq == rhs.from_sq && lhs.to_sq == rhs.to_sq;
}

std::ostream &operator<<(std::ostream &os, const Move &m)
{
  if (m.from_sq == null_square) {
    os << "0000";
    return os;
  }
  print_square(os, m.from_sq);
  print_square(os, m.to_sq);
  if (m.prom_p != null_piece) {
    os << fen::get_char_code({m.prom_p, black}); /// black for lowercase
  }
  return os;
}

void move(Board &b, const Move &m)
{
  // move pieces
  clear_sq(b, m.from_sq);
  if (m.flag == promotion || m.flag == prom_capture) {
    set_sq(b, m.to_sq, prom_piece(m));
  }
  else {
    set_sq(b, m.to_sq, from_piece(m));
  }
  if (m.flag == en_passant_capture) {
    const auto sq = m.to_sq + (b.stm() == white ? S : N);
    clear_sq(b, sq);
  }
  else if (m.flag == castle) {
    if (b.stm() == white) {
      b.king_sq(white) = m.to_sq;
    }
    else {
      b.king_sq(black) = m.to_sq;
    }
    finish_castle(b, m.to_sq);
  }
  else {
  }

  // update king position
  // second condition is to prevent reversal of hash
  if (m.from_piece.piece_t == king && m.flag != castle) {
    if (b.stm() == white) {
      b.king_sq(white) = m.to_sq;
      rm_castle_rights(b, white);
    }
    else {
      b.king_sq(black) = m.to_sq;
      rm_castle_rights(b, black);
    }
  }

  // update castling rights
  if (b.cr() != 0) {
    if (m.from_piece.piece_t == rook) {
      rook_mv_castle_rights(b, m.from_sq);
    }
    if (m.cap_piece == rook) {
      rook_mv_castle_rights(b, m.to_sq);
    }
  }

  b.t_hash() ^= zobrist.cr[m.prev_cr];
  b.t_hash() ^= zobrist.cr[b.cr()];

  if (b.ep() != null_square) {
    b.t_hash() ^= zobrist.ep[b.ep()]; // undo the previous
  }

  // update en_passant target
  if (m.flag == double_push) {
    b.ep() = m.ep_target;
    b.t_hash() ^= zobrist.ep[b.ep()];
  }
  else {
    b.ep() = null_square;
  }

  // update fmc
  if (b.stm() == black) {
    ++b.fmc();
  }

  // hmc
  if (m.from_piece.piece_t == pawn || m.flag == capture) {
    b.hmc() = 0;
  }
  else {
    ++b.hmc();
  }

  // update side to move
  b.stm() = ~b.stm();
  b.t_hash() ^= zobrist.stm;

  // update rep_stack
  b.rep_stack().push_back(b.t_hash());

  // update phase
  b.phase() = set_phase(b);
}

void unmove(Board &b, const Move &m)
{
  // move pieces
  set_sq(b, m.from_sq, from_piece(m));
  if (m.flag == en_passant_capture) {
    const auto sq = m.to_sq + (b.stm() == black ? S : N);
    clear_sq(b, m.to_sq);
    set_sq(b, sq, {pawn, b.stm()});
  }
  else if (m.cap_piece != null_piece) { // cannot be m.flag == capture (?)
    set_sq(b, m.to_sq, captured_piece(m));
  }
  else {
    clear_sq(b, m.to_sq);
  }
  if (m.flag == castle) {
    unfinish_castle(b, m.to_sq);
  }

  // update king position
  if (m.from_piece.piece_t == king) {
    if (~b.stm() == white) {
      b.king_sq(white) = m.from_sq;
    }
    else {
      b.king_sq(black) = m.from_sq;
    }
  }

  // update castling rights
  b.t_hash() ^= zobrist.cr[m.prev_cr];
  b.t_hash() ^= zobrist.cr[b.cr()];
  b.cr() = m.prev_cr;

  // en passant
  if (b.ep() != null_square) {
    b.t_hash() ^= zobrist.ep[b.ep()];
  }
  if (m.prev_ep != null_square) {
    b.t_hash() ^= zobrist.ep[m.prev_ep];
  }
  b.ep() = m.prev_ep;

  // todo update hmc
  if (b.stm() == white) {
    --b.fmc();
  }

  b.hmc() = m.prev_hmc;

  // update side to move
  b.stm() = ~b.stm();
  b.t_hash() ^= zobrist.stm;

  // update rep_stack
  b.rep_stack().pop_back();

  // update phase
  b.phase() = set_phase(b);
}

//------------------------------------------------------------------------------

PieceInfo captured_piece(const Move &m) { return PieceInfo{m.cap_piece, ~m.from_piece.color}; }

PieceInfo from_piece(const Move &m) { return PieceInfo{m.from_piece.piece_t, m.from_piece.color}; }

PieceInfo prom_piece(const Move &m) { return PieceInfo{m.prom_p, m.from_piece.color}; }

void clear_sq(Board &b, const Square sq)
{
  if (auto [piece_t, color] = b.piece_info(sq); piece_t != null_piece) {
    b.piece_on(sq) = null_piece;                   // clear
    b.color_on(sq) = null_color;                   // clear
    b.t_hash() ^= zobrist.pcs[piece_t][color][sq]; // update hash
    // b.m_hash()-= zobrist.mat[piece_t][color];     // update m_hash
    b.mat_bal(color) -= piece_val[piece_t];
    --b.mat_cnt({piece_t, color});
    b.pos_bal(color) -= psqt_val[color][piece_t][sq];
  }
  else {
    // no action needed
  }
}

/// @note calls clear square to maintain Zobrist hash
void set_sq(Board &b, const Square sq, const PieceInfo pi)
{
  if (is_on_board(sq)) { // todo when are we passed an invalid square?
    clear_sq(b, sq);
    b.piece_on(sq) = pi.piece_t;
    b.color_on(sq) = pi.color;
    assert(pi.piece_t != null_piece);
    b.t_hash() ^= zobrist.pcs[pi.piece_t][pi.color][sq];
    // b.m_hash()+= zobrist.mat[pi.piece_t][pi.color];
    b.mat_bal(pi.color) += piece_val[pi.piece_t];
    ++b.mat_cnt(pi);
    b.pos_bal(pi.color) += psqt_val[pi.color][pi.piece_t][sq];
  }
}

void finish_castle(Board &b, const Square to)
{
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
    throw std::invalid_argument("finish_castle(): invalid Square `to`");
  }
}

void unfinish_castle(Board &b, const Square to)
{
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
    throw std::invalid_argument("unfinish_castle(): invalid Square `to`");
  }
}

void rm_castle_rights(Board &b, const Color c)
{
  if (c == white) {
    b.cr() &= 0b1100;
  }
  else if (c == black) {
    b.cr() &= 0b0011;
  }
}

void rook_mv_castle_rights(Board &b, const Square sq)
{
  switch (sq) {
  case a1:
    b.cr() &= 0b1101;
    break;
  case h1:
    b.cr() &= 0b1110;
    break;
  case a8:
    b.cr() &= 0b0111;
    break;
  case h8:
    b.cr() &= 0b1011;
    break;
  default:
    break;
  }
}

std::ostream &print_square(std::ostream &os, const Square sq)
{
  const char file = static_cast<char>('a' + (sq & 0x7));
  const char rank = static_cast<char>('1' + ((sq >> 4) & 0x7));
  os.put(file).put(rank);
  return os;
}

} // namespace raab_bot
