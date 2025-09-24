module;

#include "transposition.hpp"

#include <algorithm>
#include <array>
#include <limits>

export module Board0x88:search;
import :eval;
import :move;
import :movegen;

//------------------------------------------------------------------------------

export auto
alpha_beta_root(Board &b, int &alpha, int &beta, std::uint8_t depth,
                std::chrono::time_point<std::chrono::steady_clock> start,
                long time, long &time_elapsed) -> Move;

export auto alpha_beta(Board &b, int alpha, int beta, std::uint8_t depth,
                       std::uint8_t ply) -> int;

export auto quiesce(Board &b, int alpha, int beta, std::uint8_t ply) -> int;

//------------------------------------------------------------------------------

Move alpha_beta_root(Board &b, int &alpha, int &beta, const std::uint8_t depth,
                     std::chrono::time_point<std::chrono::steady_clock> start,
                     long time, long &time_elapsed) {
  const auto node_hash = b.hash;
  const auto orig_alpha = alpha;
  std::array<Move, 256> ml{};
  Move best_move{};
  int best = std::numeric_limits<int>::min();
  int score{};
  int move_n{};
  int legal_moves{};
  for (const auto sz = movegen(b, ml.begin()); move_n < sz; ++move_n) {
    movegen_sort(std::next(ml.begin(), move_n), sz - move_n);
    const Move m = ml[move_n];
    move(b, m);
    if (is_legal(b)) {
      constexpr std::uint8_t ply{};
      score = -alpha_beta(b, -beta, -alpha, depth - 1, ply + 1);
      ++legal_moves;
      if (score > best) {
        best = score;
        best_move = m;
        alpha = std::max(score, alpha);
      }
      if (score >= beta) {
        if (auto &e = tt[node_hash & tt_mask];
            e.hash != node_hash || e.depth < depth) {
          e = {
              node_hash, {m.from_sq, m.to_sq, m.prom_p}, score, tt_beta, depth};
        }
        unmove(b, m);
        return best_move;
      }
    }
    unmove(b, m);
    time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::steady_clock::now() - start)
                       .count();
    // if (time_elapsed > time) {
    //   break;
    // }
  }
  if (legal_moves == 0) {
    return Move{};
  }
  const TT_flag flag = (best <= orig_alpha) ? tt_alpha : tt_exact;
  if (auto &e = tt[node_hash & tt_mask];
      e.hash != node_hash || e.depth < depth) {
    e = {node_hash,
         {best_move.from_sq, best_move.to_sq, best_move.prom_p},
         best,
         flag,
         depth};
  }
  return best_move;
}

int alpha_beta(Board &b, int alpha, const int beta, const std::uint8_t depth,
               const std::uint8_t ply) {
  if (depth == 0) {
    return quiesce(b, alpha, beta, ply);
  }
  const auto node_hash = b.hash;
  const auto orig_alpha = alpha;
  if (is_repetition(node_hash)) {
    if (auto &e = tt[node_hash & tt_mask];
        e.hash != node_hash || e.depth < depth) {
      e = {node_hash, {}, contempt(b), tt_exact, depth};
    }
    return contempt(b);
  }
  TT_move tt_move{};
  {
    if (const auto &e = tt[node_hash & tt_mask]; e.hash == node_hash) {
      tt_move = e.tt_m;
      if (e.depth >= depth) {
        if (e.flag == tt_exact) {
          return e.score;
        }
        if (e.flag == tt_alpha && e.score <= alpha) {
          return alpha;
        }
        if (e.flag == tt_beta && e.score >= beta) {
          return beta;
        }
      }
    }
  }

  std::array<Move, 256> ml{};
  Move best_move{};
  int best = std::numeric_limits<int>::min();
  int score{};
  int move_n{};
  int legal_moves{};
  for (const auto sz = movegen(b, ml.begin()); move_n < sz; ++move_n) {
    movegen_sort(std::next(ml.begin(), move_n), sz - move_n, tt_move);
    const Move m = ml[move_n];
    move(b, m);
    if (is_legal(b)) {
      score = -alpha_beta(b, -beta, -alpha, depth - 1, ply + 1);
      ++legal_moves;
      if (score > best) {
        best = score;
        if (score > alpha) {
          alpha = score;
          best_move = m;
        }
      }
      if (score >= beta) {
        if (auto &e = tt[node_hash & tt_mask];
            e.hash != node_hash || e.depth < depth) {
          e = {
              node_hash, {m.from_sq, m.to_sq, m.prom_p}, score, tt_beta, depth};
        }
        unmove(b, m);
        return best;
      }
    }
    unmove(b, m);
  }
  if (legal_moves == 0) {
    const int val = in_check(b) ? -(CHECKMATE - ply) : 0;
    if (auto &e = tt[node_hash & tt_mask];
        e.hash != node_hash || e.depth < depth) {
      e = {node_hash, {}, val, tt_exact, depth};
    }
    return val;
  }
  const TT_flag flag = (best <= orig_alpha) ? tt_alpha : tt_exact;
  if (auto &e = tt[node_hash & tt_mask];
      e.hash != node_hash || e.depth < depth) {
    e = {node_hash,
         {best_move.from_sq, best_move.to_sq, best_move.prom_p},
         best,
         flag,
         depth};
  }
  return best;
}

int quiesce(Board &b, int alpha, const int beta, const std::uint8_t ply) {
  if (is_repetition(b.hash)) {
    return contempt(b);
  }
  int best = static_eval(b, alpha, beta, ply);
  if (best >= beta) {
    return best;
  }
  if (best > alpha) {
    alpha = best;
  }
  std::array<Move, 256> ml{};
  int score{};
  int move_n{};
  for (const auto sz = quiescence_movegen(b, ml.begin()); move_n < sz;
       ++move_n) {
    movegen_sort(std::next(ml.begin(), move_n), sz - move_n);
    const Move m = ml[move_n];
    move(b, m);
    if (is_legal(b)) {
      score = -quiesce(b, -beta, -alpha, ply + 1);
      if (score > best) {
        best = score;
        alpha = std::max(score, alpha);
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
