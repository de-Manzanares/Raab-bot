module;

#include <iostream>
#include <string_view>

import fen;
import transposition;
import types;
import eval;

module board;

using svci = std::string_view::const_iterator;

Board::Board(const std::string_view fenstr)
{
  using namespace fen;
  piece_on.fill(null_piece);
  color_on.fill(null_color);
  auto it  = fenstr.begin();
  int  idx = 0;
  for (; idx < 64; ++it) {
    if (is_piece(*it)) {
      const auto [piece_t, color] = get_piece_info(*it);
      const auto sq               = to_0x88_idx(idx);
      piece_on[sq]                = piece_t;
      color_on[sq]                = color;
      if (piece_t == king) {
        if (color == white) {
          wks = Square{sq};
        }
        else if (color == black) {
          bks = Square{sq};
        }
      }
      idx++;
      t_hash ^= zobrist.pcs[piece_t][color][sq];
      // m_hash+= zobrist.mat[piece_t][color];
      mat_bal[color] += piece_val[piece_t];
      ++mat_cnt[color][piece_t];
      pos_bal[color] += psqt_val[color][piece_t][sq];
    }
    else if (*it >= '1' && *it <= '8') { // empty squares
      idx += *it - '0';
    }
    else if (*it == ' ') {
      break;
    }
  }
  // minimal error checking
  // did we get all the squares?
  if (idx != 64) { // todo clean this up
    throw std::runtime_error("malformed FEN string");
  }

  ++it;
  if (*it == 'w') {
    stm = white;
  }
  else {
    stm = black;
    t_hash ^= stm;
  }
  std::advance(it, 2);

  if (*it == '-') {
    cr = 0;
    ++it;
  }
  else {
    for (; it != fenstr.end() && *it != ' '; ++it) {
      switch (*it) {
      case 'K':
        cr += 0b0001;
        break;
      case 'Q':
        cr += 0b0010;
        break;
      case 'k':
        cr += 0b0100;
        break;
      case 'q':
        cr += 0b1000;
        break;
      default:;
      }
    }
  }
  t_hash ^= zobrist.cr[cr];

  ++it;
  if (*it == '-') {
    ep = null_square;
    std::advance(it, 2);
  }
  else {
    ep = static_cast<Square>((16 * (*std::next(it) - '0' - 1)) + *it - 'a');
    std::advance(it, 3);
    t_hash ^= zobrist.ep[ep];
  }

  if (it == fenstr.end() || std::next(it) == fenstr.end()) {
    return;
  }

  hmc = *it - '0';
  fmc = *std::next(it, 2) - '0';

  tt[t_hash & tt_mask] = {t_hash, {}, {}, {}, {}};
  mt[m_hash & mt_mask] = {m_hash, mat_bal[white] - mat_bal[black]};

  phase = set_phase(*this);
}

PieceInfo Board::piece_info(const Square sq) const
{
  if (is_on_board(sq)) { // todo when are we passed an invalid square?
    return {.piece_t = piece_on[sq], .color = color_on[sq]};
  }
  return {null_piece, null_color};
}

void Board::history_decay()
{
  for (int c = 0; c < 2; ++c) {
    for (int from = 0; from < 128; ++from) {
      for (int to = 0; to < 128; ++to) {
        history[c][from][to] -= (history[c][from][to] >> 5);
      }
    }
  }
}

void Board::reset() { *this = Board(fen::startpos); }

void Board::display() const
{
  for (int idx = 0; idx < 64; ++idx) {
    const auto      sq = fen::to_0x88_idx(idx);
    const PieceInfo pi{.piece_t = piece_on[sq], .color = color_on[sq]};
    const char      ch = fen::get_char_code(pi);
    std::cout << ' ' << ch;
    if ((idx + 1) % 8 == 0) {
      std::cout << '\n';
    }
  }
  std::cout << '\n';
  std::cout << "stm: ";
  if (stm == white) {
    std::cout << "white ";
  }
  else {
    std::cout << "black ";
  }
  std::cout << "cr: ";
  if (cr & 0b0001) {
    std::cout << "K";
  }
  if (cr & 0b0010) {
    std::cout << "Q";
  }
  if (cr & 0b0100) {
    std::cout << "k";
  }
  if (cr & 0b1000) {
    std::cout << "q";
  }
  if (cr == 0) {
    std::cout << '-';
  }
  std::cout << " ep: ";
  if (ep != null_square) {
    std::cout << ep;
  }
  else {
    std::cout << "- ";
  }
  std::cout << "fmc: " << fmc;
  std::cout << std::endl;
}
