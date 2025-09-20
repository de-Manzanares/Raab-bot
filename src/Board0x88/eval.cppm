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

constexpr int piece_vals[6] = {0, 9, 5, 3, 3, 1};

bool in_check(const Board &b) {
  const Square sq =
      b.stm == white ? static_cast<Square>(b.wks) : static_cast<Square>(b.bks);
  return is_attacked(b, sq, ~b.stm);
}

enum terminus_t { stalemate, checkmate, null_terminus };

struct Terminus {
  terminus_t term_t;
  int val{};
};

// todo calling movegen twice per search depth seems less than optimal

constexpr Terminus terminus_check(Board &b) {
  if (cnt_legal_moves(b) != 0) {
    return {null_terminus, 0};
  }
  if (in_check(b)) {
    return {checkmate, CHECKMATE};
  }
  if (!in_check(b)) {
    return {stalemate, 0};
  }
}

constexpr score_t material(const Board &b) {
  int mat{};
  for (const auto sq : square_sequence) {
    if (b.piece_on[sq] != null_piece) {
      auto [piece_t, color] = b.piece_info(sq);
      const int mult = color == white ? 1 : -1;
      mat += piece_vals[piece_t] * mult;
    }
  }
  return mat;
}

export constexpr score_t eval(const Board &b) {
  const auto score = material(b);
  return score * (b.stm == white ? 1 : -1);
}
