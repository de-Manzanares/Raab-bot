module;

#include <array>
#include <cstdint>
#include <limits>

export module Board0x88:search;
import :eval;
import :move;
import :movegen;

export int negamax(Board &b, std::uint8_t depth) {
  if (is_checkmate(b)) {
    return b.stm == white ? -CHECKMATE : CHECKMATE;
  }
  if (depth == 0) {
    return eval(b);
  }
  int score{};
  int max = std::numeric_limits<int>::min();
  std::array<Move, 256> ml{};
  movegen(b, ml.begin());
  for (const auto &m : ml) {
    if (m.from_sq == null_square) {
      break;
    }
    move(b, m);
    if (is_legal(b)) {
      score = -negamax(b, depth - 1);
      if (score > max) {
        max = score;
      }
      max = std::max(max, score);
    }
    un_move(b, m);
  }
  return max;
}

export Move negamax_root(Board &b, std::uint8_t depth) {
  Move best_move{};
  int score{};
  int max = std::numeric_limits<int>::min();
  std::array<Move, 256> ml{};
  movegen(b, ml.begin());
  for (const auto &m : ml) {
    if (m.from_sq == null_square) {
      break;
    }
    move(b, m);
    if (is_legal(b)) {
      score = -negamax(b, depth - 1);
      if (score > max) {
        max = score;
        best_move = m;
      }
    }
    un_move(b, m);
  }
  return best_move;
}