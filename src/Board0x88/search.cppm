module;

#include <array>
#include <cstdint>
#include <limits>
#include <vector>

export module Board0x88:search;
import :eval;
import :move;
import :movegen;

export int negamax(Board &b, const std::uint8_t depth) {
  switch (auto [term_t, term_v] = terminus_check(b); term_t) {
  case checkmate:
    return -term_v;
  case stalemate:
    return term_v;
  default:;
  }

  if (depth == 0) {
    return eval(b);
  }

  int score{};
  int max = std::numeric_limits<int>::min();
  std::array<Move, 256> ml{};
  int move_n{};

  for (const auto sz = movegen(b, ml.begin()); move_n < sz; ++move_n) {
    movegen_sort(std::next(ml.begin(), move_n), sz);
    const Move m = ml[move_n];
    move(b, m);
    if (is_legal(b)) {
      score = -negamax(b, depth - 1);
      if (score > max) {
        max = score;
      }
    }
    unmove(b, m);
  }

  return max;
}

export Move negamax_root(Board &b, const std::uint8_t depth) {
  // if the root node is already checkmate or stalemate, we done
  switch (auto [term_t, term_v] = terminus_check(b); term_t) {
  case checkmate:
    return Move{};
  case stalemate:
    return Move{};
  default:;
  }

  Move best_move{};
  int score{};
  int max = std::numeric_limits<int>::min();
  std::array<Move, 256> ml{};
  int move_n{};

  for (const auto sz = movegen(b, ml.begin()); move_n < sz; ++move_n) {
    movegen_sort(std::next(ml.begin(), move_n), sz);
    const Move m = ml[move_n];
    move(b, m);
    if (is_legal(b)) {
      score = -negamax(b, depth - 1);
      if (score > max) {
        max = score;
        best_move = m;
      }
    }
    unmove(b, m);
  }

  return best_move;
}