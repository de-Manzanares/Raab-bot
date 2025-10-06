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
  auto lte_one_minor_piece = [&](const Color c) {
    int sum{};
    for (int p = bishop; p <= knight; ++p) {
      sum += b.mat_cnt[c][p];
      if (sum > 1) {
        return false;
      }
    }
    return true;
  };

  Phase phase{};
  if (bool no_queens = b.mat_cnt[white][queen] == 0 && b.mat_cnt[black][queen] == 0) {
    phase = end_game;
  }
  else {
    bool cnd = true;
    for (int color = white; color <= black; ++color) {
      if (b.mat_cnt[color][queen] > 0) {
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
  score_t score = (b.mat_bal[white] - b.mat_bal[black]) + (b.pos_bal[white] - b.pos_bal[black]);
  if (b.phase == end_game) {
    score -= psqt_val[white][king][b.wks];
    score -= psqt_val[black][king][b.bks];
    score += eg_psqt[white][b.wks];
    score += eg_psqt[black][b.bks];
  }
  return b.stm == white ? score : -score;
}

bool is_repetition(const Board &b)
{
  auto const last = b.pos_stack.end() - 1;
  if (const auto first = std::find(b.pos_stack.begin(), last, b.t_hash); first != last) {
    if (const auto second = std::find(first, last, b.t_hash); second != last) {
      return true;
    }
  }
  return false;
}

score_t contempt(const Board &b)
{
  return (b.mat_bal[white] - b.mat_bal[black]) * (b.stm == white ? -1 : 1);
}

//------------------------------------------------------------------------------

} // namespace raab_bot
