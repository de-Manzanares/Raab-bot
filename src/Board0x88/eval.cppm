module;

#include <cstddef>
#include <iterator>
#include <limits>

export module Board0x88:eval;
import :core;
import :move;
import :movegen;

export using score_t = int;

constexpr score_t CHECKMATE = 100'000'000;

bool in_check(const Board &b) {
  const Square sq =
      b.stm == white ? static_cast<Square>(b.wks) : static_cast<Square>(b.wks);
  return is_attacked(b, sq, ~b.stm);
}

constexpr bool is_checkmate(Board &b) {
  if (in_check(b) && cnt_legal_moves(b) == 0) {
    return true;
  }
  return false;
}

constexpr score_t material(Board &b) { return 0; }

constexpr score_t eval(Board &b) {
  const auto score = material(b) * (b.stm == white ? 1 : -1);
  return score;
}
