module;

#include <algorithm>

import attack;
import board;
import defs;
import movegen;
import transposition;

module eval;

namespace raab_bot {

//------------------------------------------------------------------------------

Phase set_phase(const Board &b)
{
  auto lte_one_minor_piece = [&](const Color c) -> bool {
    int sum{};
    for (int p = bishop; p <= knight; ++p) {
      // todo Piece - overload? conversion? ++/-- and comp operators?
      sum += b.mat_cnt({static_cast<Piece>(p), static_cast<Color>(c)});
      if (sum > 1) {
        return false;
      }
    }
    return true;
  };

  auto no_queens = [&]() -> bool {
    return b.mat_cnt({queen, white}) == 0 && b.mat_cnt({queen, black}) == 0;
  };

  Phase phase{};

  if (no_queens()) {
    phase = end_game;
  }
  else {
    bool cnd = true;
    for (int color = white; color <= black; ++color) {
      // todo Color - conversion? overload mat_cnt?
      if (b.mat_cnt({queen, static_cast<Color>(color)}) > 0) {
        if (!lte_one_minor_piece(static_cast<Color>(color))) {
          cnd = false;
        }
      }
    }
    if (cnd) {
      phase = end_game;
    }
  }
  return phase;
}

score_t tmsef(const Board &b)
{
  score_t score = b.mat_bal() + b.pos_bal();
  if (b.phase() == end_game) {
    score -= psqt_val[white][king][b.king_sq(white)];
    score -= psqt_val[black][king][b.king_sq(black)];
    score += eg_psqt[white][b.king_sq(white)];
    score += eg_psqt[black][b.king_sq(black)];
  }
  return b.stm() == white ? score : -score;
}

bool is_repetition(const Board &b)
{
  auto const last = b.rep_stack().end() - 1;
  if (const auto first = std::find(b.rep_stack().begin(), last, b.t_hash()); first != last) {
    if (const auto second = std::find(first, last, b.t_hash()); second != last) {
      return true;
    }
  }
  return false;
}

score_t contempt(const Board &b) { return b.mat_bal() * (b.stm() == white ? -1 : 1); }

//------------------------------------------------------------------------------

} // namespace raab_bot
