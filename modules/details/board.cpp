module;

#include <iostream>
#include <string_view>

import config;
import defs;
import fen;
import transposition;
import eval;

module board;

namespace raab_bot {

using svci = std::string_view::const_iterator;

Board::Board(const std::string_view fenstr)
{
  using namespace fen;
  piece_on_.fill(null_piece);
  color_on_.fill(null_color);
  auto it  = fenstr.begin();
  int  idx = 0;
  for (; idx < 64; ++it) {
    if (is_piece(*it)) {
      const auto [piece_t, color] = get_piece_info(*it);
      const auto sq               = to_0x88_idx(idx);
      piece_on_[sq]               = piece_t;
      color_on_[sq]               = color;
      if (piece_t == king) {
        if (color == white) {
          wks_ = Square{sq};
        }
        else if (color == black) {
          bks_ = Square{sq};
        }
      }
      idx++;
      t_hash_ ^= zobrist.pcs[piece_t][color][sq];
      // m_hash+= zobrist.mat[piece_t][color];
      mat_bal(color) += piece_val[piece_t];
      // todo fix order of data in PieceInfo lol
      ++mat_cnt({piece_t, color});
      pos_bal(color) += psqt_val[color][piece_t][sq];
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
    stm_ = white;
  }
  else {
    stm_ = black;
    t_hash_ ^= stm_;
  }
  std::advance(it, 2);

  if (*it == '-') {
    cr_ = 0;
    ++it;
  }
  else {
    for (; it != fenstr.end() && *it != ' '; ++it) {
      switch (*it) {
      case 'K':
        cr_ += 0b0001;
        break;
      case 'Q':
        cr_ += 0b0010;
        break;
      case 'k':
        cr_ += 0b0100;
        break;
      case 'q':
        cr_ += 0b1000;
        break;
      default:;
      }
    }
  }
  t_hash_ ^= zobrist.cr[cr_];

  ++it;
  if (*it == '-') {
    ep_ = null_square;
    std::advance(it, 2);
  }
  else {
    ep_ = static_cast<Square>(16 * (*std::next(it) - '0' - 1) + *it - 'a');
    std::advance(it, 3);
    t_hash_ ^= zobrist.ep[ep_];
  }

  if (it == fenstr.end() || std::next(it) == fenstr.end()) {
    return;
  }

  hmc_ = *it - '0';
  fmc_ = *std::next(it, 2) - '0';

  tt[t_hash_ & tt_mask] = {t_hash_, {}, {}, {}, {}};
  mt[m_hash_ & mt_mask] = {m_hash_, mat_bal()};

  phase_ = set_phase(*this);
  rep_stack().reserve(64);
}

PieceInfo Board::piece_info(const Square sq) const
{
  if (is_on_board(sq)) { // todo when are we passed an invalid square?
    return {.piece_t = piece_on_[sq], .color = color_on_[sq]};
  }
  return {null_piece, null_color};
}

void Board::reset() { *this = Board(fen::startpos); }

void Board::display() const
{
  for (int idx = 0; idx < 64; ++idx) {
    const auto      sq = fen::to_0x88_idx(idx);
    const PieceInfo pi{.piece_t = piece_on_[sq], .color = color_on_[sq]};
    const char      ch = fen::get_char_code(pi);
    std::cout << ' ' << ch;
    if ((idx + 1) % 8 == 0) {
      std::cout << '\n';
    }
  }
  std::cout << '\n';
  std::cout << "stm_: ";
  if (stm_ == white) {
    std::cout << "white ";
  }
  else {
    std::cout << "black ";
  }
  std::cout << "cr_: ";
  if (cr_ & 0b0001) {
    std::cout << "K";
  }
  if (cr_ & 0b0010) {
    std::cout << "Q";
  }
  if (cr_ & 0b0100) {
    std::cout << "k";
  }
  if (cr_ & 0b1000) {
    std::cout << "q";
  }
  if (cr_ == 0) {
    std::cout << '-';
  }
  std::cout << " ep: ";
  if (ep_ != null_square) {
    std::cout << ep_;
  }
  else {
    std::cout << "- ";
  }
  std::cout << "fmc: " << fmc_;
  std::cout << std::endl;
}

} // namespace raab_bot
