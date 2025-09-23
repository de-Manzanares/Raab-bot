module;

#include "transposition.hpp"

#include <array>
#include <cstdint>
#include <limits>
#include <vector>

export module Board0x88:search;
import :eval;
import :move;
import :movegen;

// TODO cpw has a great terminal detection strategy

export int quiesce(Board &b, int alpha, int beta, std::uint8_t ply) {
  int best = static_eval(b, ply);
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
      score = -quiesce(b, -beta, -alpha, ply + 1);
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

export int alpha_beta(Board &b, int alpha, int beta, const std::uint8_t depth,
                      const std::uint8_t ply) {
  if (depth == 0) {
    return quiesce(b, alpha, beta, ply);
  }

  const auto node_hash = b.hash;
  const auto orig_alpha = alpha;
  TT_move tt_move{};
  {
    const auto &e = tt[node_hash & tt_mask];
    if (e.hash == node_hash) {
      tt_move = e.tt_m;
      if (e.depth >= depth) {
        if (e.flag == tt_exact)
          return e.score;
        if (e.flag == tt_alpha && e.score <= alpha)
          return alpha;
        if (e.flag == tt_beta && e.score >= beta)
          return beta;
      }
    }
  }

  int score{};
  int best = std::numeric_limits<int>::min();
  std::array<Move, 256> ml{};
  int move_n{};
  int legal_moves{};
  Move best_move{};

  for (const auto sz = movegen(b, ml.begin()); move_n < sz; ++move_n) {
    movegen_sort(std::next(ml.begin(), move_n), sz - move_n, tt_move);
    const Move m = ml[move_n];
    move(b, m);
    if (is_legal(b)) {
      ++legal_moves;
      score = -alpha_beta(b, -beta, -alpha, depth - 1, ply + 1);
      if (score > best) {
        best = score;
        if (score > alpha) {
          alpha = score;
          best_move = m;
        }
      }
      if (score >= beta) {
        auto &e = tt[node_hash & tt_mask];
        if (e.hash != node_hash || e.depth < depth) {
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
    auto &e = tt[node_hash & tt_mask];
    if (e.hash != node_hash || e.depth < depth) {
      e = {node_hash, {}, val, tt_exact, depth};
    }
    return val;
  }
  const TT_flag flag = (best <= orig_alpha) ? tt_alpha : tt_exact;
  auto &e = tt[node_hash & tt_mask];
  if (e.hash != node_hash || e.depth < depth) {
    e = {node_hash,
         {best_move.from_sq, best_move.to_sq, best_move.prom_p},
         best,
         flag,
         depth};
  }
  return best;
}

export Move alpha_beta_root(Board &b, int alpha, int beta,
                            const std::uint8_t depth) {
  const auto node_hash = b.hash;
  const auto orig_alpha = alpha;

  const std::uint8_t ply{};
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
      score = -alpha_beta(b, -beta, -alpha, depth - 1, ply + 1);
      if (score > best) {
        best = score;
        best_move = m; // bestmove here? or below?
        if (score > alpha) {
          alpha = score;
        }
      }
      if (score >= beta) {
        auto &e = tt[node_hash & tt_mask];
        if (e.hash != node_hash || e.depth < depth) {
          e = {
              node_hash, {m.from_sq, m.to_sq, m.prom_p}, score, tt_beta, depth};
        }
        unmove(b, m);
        return best_move;
      }
    }
    unmove(b, m);
  }
  if (legal_moves == 0) {
    return Move{};
  }
  const TT_flag flag = (best <= orig_alpha) ? tt_alpha : tt_exact;
  auto &e = tt[node_hash & tt_mask];
  if (e.hash != node_hash || e.depth < depth) {
    e = {node_hash,
         {best_move.from_sq, best_move.to_sq, best_move.prom_p},
         best,
         flag,
         depth};
  }
  return best_move;
}
