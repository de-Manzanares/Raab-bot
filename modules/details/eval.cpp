module;

import attack;
import board;
import movegen;
import types;
import transposition;

module eval;

score_t check_bonus(const Board &b);
score_t king_restriction(Board &b);
score_t material(const Board &b);
score_t mobility(const Board &b);

//------------------------------------------------------------------------------

score_t static_eval(Board &b)
{
  // todo dedicated eval cache
  const auto cb  = check_bonus(b);
  const auto kr  = king_restriction(b) * 8;
  const auto mat = material(b) * 128;
  const auto mob = mobility(b) >> 3;
  const auto sum = cb + kr + mat + mob;
  return b.stm == white ? sum : -sum;
}

score_t contempt(const Board &b)
{
  return (material(b) * (b.stm == white ? -1 : 1) * 128);
}

//------------------------------------------------------------------------------

score_t check_bonus(const Board &b)
{
  score_t score{};
  if (b.stm == white) {
    if (is_attacked(b, b.wks, black)) {
      score = -16;
    }
  }
  else {
    if (is_attacked(b, b.bks, white)) {
      score = 16;
    }
  }
  return score;
}

// the lower, the better, so we return the negated value
score_t king_restriction(Board &b)
{
  auto ratio = [&b](const Square ksq, const Color c) {
    double  att{};
    score_t valid{};
    for (const auto dir : unit_vectors[king]) {
      if (is_on_board(ksq + dir)) {
        ++valid;
      }
      if (is_attacked(b, ksq + dir, c)) {
        ++att;
      }
    }
    return att / valid;
  };
  const auto w  = ratio(b.wks, black);
  const auto bl = ratio(b.bks, white);
  return -(w - bl);
}

constexpr score_t piece_vals[6] = {0, 9, 5, 3, 3, 1};

score_t material(const Board &b)
{
  if (const auto &e = mt[b.m_hash & mt_mask]; e.m_hash == b.m_hash) {
    return e.mat;
  }
  score_t mat{};
  for (const auto sq : square_sequence) {
    if (b.piece_on[sq] != null_piece) {
      auto [piece_t, color] = b.piece_info(sq);
      const score_t mult    = color == white ? 1 : -1;
      mat += piece_vals[piece_t] * mult;
    }
  }
  mt[b.m_hash & mt_mask] = {b.m_hash, mat};
  return mat;
}

// todo remove uses of cnt_legal_moves();
score_t mobility(const Board &b)
{
  Board   tmp = b;
  score_t w_mob{};
  score_t b_mob{};
  tmp.stm = white;
  w_mob   = cnt_legal_moves(tmp);
  tmp.stm = black;
  b_mob   = cnt_legal_moves(tmp);
  return w_mob - b_mob;
}
