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
  int best = static_eval(b);
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
  if (depth == 0) {
    return quiesce(b, alpha, beta);
  }

  int score{};
  int best = std::numeric_limits<int>::min();
  std::array<Move, 256> ml{};
  int move_n{};
  int legal_moves{};

  for (const auto sz = movegen(b, ml.begin()); move_n < sz; ++move_n) {
    movegen_sort(std::next(ml.begin(), move_n), sz - move_n);
    const Move m = ml[move_n];
    move(b, m);
    if (is_legal(b)) {
      ++legal_moves;
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

  if (legal_moves == 0) {
    if (bool checkmate = in_check(b)) {
      return -CHECKMATE;
    }
    return 0; // stalemate
  }

  return best;
}

export Move alpha_beta_root(Board &b, int alpha, int beta,
                            const std::uint8_t depth) {
  Move best_move{};
  int score{};
  int best = std::numeric_limits<int>::min();
  std::array<Move, 256> ml{};
  int move_n{};
  int legal_moves{};

  for (const auto sz = movegen(b, ml.begin()); move_n < sz; ++move_n) {
    movegen_sort(std::next(ml.begin(), move_n), sz - move_n);
    const Move m = ml[move_n];
    move(b, m);
    if (is_legal(b)) {
      ++legal_moves;
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
  if (legal_moves == 0) {
    return Move{};
  }
  return best_move;
}