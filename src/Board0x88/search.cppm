module;

#include <array>
#include <cstdint>
#include <limits>
#include <vector>

export module Board0x88:search;
import :eval;
import :move;
import :movegen;

export int quiesce(Board &b, int alpha, int beta) {
  int best = eval(b);
  if (best >= beta) {
    return best;
  }
  if (best > alpha) {
    alpha = best;
  }

  int score{};
  std::array<Move, 256> ml{};
  int move_n{};
  for (const auto sz = quiescence_movegen(b, ml.begin()); move_n < sz;
       ++move_n) {
    movegen_sort(std::next(ml.begin(), move_n), sz - move_n);
    const Move m = ml[move_n];
    move(b, m);
    if (is_legal(b)) {
      score = -quiesce(b, -beta, -alpha);
      if (score > best) {
        best = score;
        if (score > alpha) {
          alpha = score;
        }
      }
      if (score >= beta) {
        unmove(b, m);
        return score;
      }
    }
    unmove(b, m);
  }

  return best;
}

export int alpha_beta(Board &b, int alpha, int beta, const std::uint8_t depth) {
  switch (auto [term_t, term_v] = terminus_check(b); term_t) {
  case checkmate:
    return -term_v;
  case stalemate:
    return term_v;
  default:;
  }

  if (depth == 0) {
    return quiesce(b, std::numeric_limits<int>::min(),
                   std::numeric_limits<int>::max());
  }

  int score{};
  int best = std::numeric_limits<int>::min();
  std::array<Move, 256> ml{};
  int move_n{};

  for (const auto sz = movegen(b, ml.begin()); move_n < sz; ++move_n) {
    movegen_sort(std::next(ml.begin(), move_n), sz - move_n);
    const Move m = ml[move_n];
    move(b, m);
    if (is_legal(b)) {
      score = -alpha_beta(b, -beta, -alpha, depth - 1);
      if (score > best) {
        best = score;
        if (score > alpha) {
          alpha = score;
        }
      }
      if (score >= beta) {
        unmove(b, m);
        return best;
      }
    }
    unmove(b, m);
  }

  return best;
}

export Move alpha_beta_root(Board &b, int alpha, int beta,
                            const std::uint8_t depth) {
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
  int best = std::numeric_limits<int>::min();
  std::array<Move, 256> ml{};
  int move_n{};

  for (const auto sz = movegen(b, ml.begin()); move_n < sz; ++move_n) {
    movegen_sort(std::next(ml.begin(), move_n), sz - move_n);
    const Move m = ml[move_n];
    move(b, m);
    if (is_legal(b)) {
      score = -alpha_beta(b, -beta, -alpha, depth - 1);
      if (score > best) {
        best = score;
        best_move = m; // bestmove here? or below?
        if (score > alpha) {
          alpha = score;
        }
      }
      if (score >= beta) {
        unmove(b, m);
        return best_move;
      }
    }
    unmove(b, m);
  }

  return best_move;
}