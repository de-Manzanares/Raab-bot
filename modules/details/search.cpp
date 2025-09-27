module;

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <limits>

import attack;
import board;
import eval;
import move;
import movegen;
import transposition;
import types;

module search;

//------------------------------------------------------------------------------

void alpha_beta_root(Board &b, score_t alpha, score_t beta, const U8 depth) {
  std::ranges::fill(g_pv, Move{});
  rte              = 0;
  root_beta_cutoff = false;
  // todo small optimization don't need to call movegen twice here
  root_trees = cnt_legal_moves(b);
  node_count = 0;

  PVLine     line;
  const auto node_hash  = b.hash;
  const auto orig_alpha = alpha;
  TT_move    tt_move{};
  {
    if (const auto &e = tt[node_hash & tt_mask]; e.hash == node_hash) {
      tt_move = e.tt_m;
    }
  }
  MoveList ml{};
  Move     best_move{};
  score_t  best = std::numeric_limits<score_t>::min();
  score_t  score{};
  sz_t     move_n{};
  sz_t     legal_moves{};

  for (const auto sz = movegen(b, ml.begin()); move_n < sz; ++move_n, ++rte) {
    movegen_sort(std::next(ml.begin(), move_n), sz - move_n, tt_move);
    const Move m = ml[move_n];
    move(b, m);
    if (is_legal(b)) {
      ++node_count;
      constexpr U8 ply{};
      score = -alpha_beta(b, -beta, -alpha, depth - 1, ply + 1, &line);
      ++legal_moves;
      if (score > best) {
        best      = score;
        best_move = m;
        if (score > alpha) {
          alpha   = score;
          g_pv[0] = m;
          auto it = std::ranges::find(line, Move{});
          std::copy(line.begin(), it, std::next(g_pv.begin()));
        }
      }
      if (score >= beta) {
        if (auto &e = tt[node_hash & tt_mask];
            e.hash != node_hash || e.depth < depth) {
          e = {
              node_hash, {m.from_sq, m.to_sq, m.prom_p}, score, tt_beta, depth};
        }
        unmove(b, m);
        root_beta_cutoff = true;
        return;
      }
    }
    unmove(b, m);
    time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::steady_clock::now() - start)
                       .count();
    if (time_elapsed > allowed_time) {
      break;
    }
  }
  if (legal_moves == 0) {
    return;
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
  return;
}

score_t alpha_beta(Board &b, score_t alpha, const score_t beta, const U8 depth,
                   const U8 ply, PVLine *pline) {
  PVLine line{};
  if (depth == 0) {
    // pline->count = 0; ?
    return quiesce(b, alpha, beta, ply, &line);
  }
  const auto node_hash  = b.hash;
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

  MoveList ml{};
  Move     best_move{};
  score_t  best = std::numeric_limits<score_t>::min();
  score_t  score{};
  sz_t     move_n{};
  sz_t     legal_moves{};
  for (const auto sz = movegen(b, ml.begin()); move_n < sz; ++move_n) {
    movegen_sort(std::next(ml.begin(), move_n), sz - move_n, tt_move);
    const Move m = ml[move_n];
    move(b, m);
    if (is_legal(b)) {
      ++node_count;
      score = -alpha_beta(b, -beta, -alpha, depth - 1, ply + 1, &line);
      ++legal_moves;
      if (score > best) {
        best = score;
        if (score > alpha) {
          alpha       = score;
          best_move   = m;
          (*pline)[0] = m;
          auto it     = std::ranges::find(line, Move{});
          std::copy(line.begin(), it, std::next(pline->begin()));
        }
      }
      if (score >= beta) {
        if (auto &e = tt[node_hash & tt_mask];
            e.hash != node_hash || e.depth < depth) {
          e = {
              node_hash, {m.from_sq, m.to_sq, m.prom_p}, score, tt_beta, depth};
        }
        unmove(b, m);
        return score;
      }
    }
    unmove(b, m);
    time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::steady_clock::now() - start)
                       .count();
    if (time_elapsed > allowed_time) {
      break;
    }
  }
  if (legal_moves == 0) {
    const score_t val = in_check(b) ? -(CHECKMATE - ply) : 0;
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

score_t quiesce(Board &b, score_t alpha, const score_t beta, const U8 ply,
                PVLine *pline) {
  PVLine line{};
  if (is_repetition(b.hash)) {
    return contempt(b);
  }
  score_t best{};
  if (in_check(b)) {
    best = alpha_beta(b, alpha, beta, 1, ply, &line);
  } else {
    best = static_eval(b);
  }
  if (best >= beta) {
    return best;
  }
  alpha = std::max(best, alpha);

  MoveList ml{};
  score_t  score{};
  sz_t     move_n{};
  for (const auto sz = quiescence_movegen(b, ml.begin()); move_n < sz;
       ++move_n) {
    movegen_sort(std::next(ml.begin(), move_n), sz - move_n);
    const Move m = ml[move_n];
    move(b, m);
    if (is_legal(b)) {
      ++node_count;
      score = -quiesce(b, -beta, -alpha, ply + 1, pline);
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

//------------------------------------------------------------------------------
