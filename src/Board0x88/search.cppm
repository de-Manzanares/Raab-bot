module;

#include "transposition.hpp"

#include <array>
#include <cstdint>
#include <exception>
#include <limits>
#include <vector>

export module Board0x88:search;
import :eval;
import :move;
import :movegen;

struct TimeUp : std::exception {
  const char *what() const noexcept override { return "time up"; }
};

// TODO cpw has a great terminal detection strategy

export template <class OutputIt>
  requires std::output_iterator<OutputIt, Move>
int quiesce(Board &b, int alpha, int beta, std::uint8_t ply, OutputIt out) {
  int best = static_eval(b, alpha, beta, ply);
  if (best >= beta) {
    return best;
  }
  if (best > alpha) {
    alpha = best;
  }

  int score{};
  int move_n{};

  for (const auto sz = quiescence_movegen(b, out); move_n < sz; ++move_n) {
    movegen_sort(std::next(out, move_n), sz - move_n);
    const Move m = *std::next(out, move_n);
    move(b, m);
    if (is_legal(b)) {
      score = -quiesce(b, -beta, -alpha, ply + 1, std::next(out, sz));
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

export template <class OutputIt>
  requires std::output_iterator<OutputIt, Move>
int alpha_beta(Board &b, int alpha, int beta, const std::uint8_t depth,
               const std::uint8_t ply, OutputIt out) {
  if (depth == 0) {
    return quiesce(b, alpha, beta, ply, out);
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
  int move_n{};
  int legal_moves{};
  Move best_move{};

  for (const auto sz = movegen(b, out); move_n < sz; ++move_n) {
    movegen_sort(std::next(out, move_n), sz - move_n, tt_move);
    const Move m = *std::next(out, move_n);
    move(b, m);
    if (is_legal(b)) {
      ++legal_moves;
      score =
          -alpha_beta(b, -beta, -alpha, depth - 1, ply + 1, std::next(out, sz));
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
  std::array<Move, 2048> ml;

  const auto node_hash = b.hash;
  const auto orig_alpha = alpha;

  const std::uint8_t ply{};
  Move best_move{};
  int score{};
  int best = std::numeric_limits<int>::min();
  int move_n{};
  int legal_moves{};

  for (const auto sz = movegen(b, ml.begin()); move_n < sz; ++move_n) {
    movegen_sort(std::next(ml.begin(), move_n), sz - move_n);
    const Move m = ml[move_n];
    move(b, m);
    if (is_legal(b)) {
      ++legal_moves;
      score = -alpha_beta(b, -beta, -alpha, depth - 1, ply + 1,
                          std::next(ml.begin(), sz));
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
