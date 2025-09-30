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

bool pv_found();
bool time_up();

//------------------------------------------------------------------------------

void alpha_beta_root(Board &b, score_t alpha, score_t beta, const U8 depth)
{
  std::ranges::fill(g_pv, Move{});
  g_eval           = 0;
  rte              = 0;
  root_beta_cutoff = false;
  // todo small optimization don't need to call movegen twice here
  root_trees = cnt_legal_moves(b);
  node_count = 0;

  const auto node_hash  = b.t_hash;
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
  sz_t     move_n{};
  sz_t     legal_moves{};

  for (const auto sz = movegen(b, ml.begin()); move_n < sz; ++move_n, ++rte) {
    PVLine line{};
    movegen_sort(b.stm, std::next(ml.begin(), move_n), sz - move_n,
                 move_n == 0 ? tt_move : TT_move{});
    const Move m = ml[move_n];
    move(b, m);
    if (is_legal(b)) {
      ++node_count;
      constexpr U8 ply{};
      g_eval = -alpha_beta(b, -beta, -alpha, depth - 1, ply + 1, &line);
      ++legal_moves;
      if (g_eval > best) {
        best      = g_eval;
        best_move = m;
        if (g_eval > alpha) {
          alpha   = g_eval;
          g_pv[0] = m;
          auto it = std::ranges::find(line, Move{});
          std::copy(line.begin(), it, std::next(g_pv.begin()));
          if (const auto pit =
                  std::next(g_pv.begin(), std::distance(line.begin(), it) + 1);
              pit != g_pv.end()) {
            *pit = Move{};
          }
        }
      }
      if (g_eval >= beta) {
        if (auto &e = tt[node_hash & tt_mask];
            e.hash != node_hash || e.depth < depth) {
          e = {
              node_hash,
              {m.from_sq, m.to_sq, m.prom_p},
              g_eval,
              tt_beta,
              depth
          };
        }
        if (m.flag != capture && m.flag != prom_capture &&
            m.flag != promotion) {
          auto      &h   = history[~b.stm][m.from_sq][m.to_sq];
          const auto inc = depth * depth;
          h += inc - (h * inc) / hmax;
        }
        unmove(b, m);
        root_beta_cutoff = true;
        return;
      }
    }
    unmove(b, m);
    if ((node_count & 127) == 0 && time_up()) {
      if (!pv_found()) {
        prev_layer_g_pv[0] = best_move;
        prev_layer_g_pv[1] = Move{};
      }
      break;
    }
  }
  if (legal_moves == 0) {
    return;
  }
  const TT_flag flag = (best <= orig_alpha) ? tt_alpha : tt_exact;
  if (auto &e = tt[node_hash & tt_mask];
      e.hash != node_hash || e.depth < depth) {
    e = {
        node_hash,
        {best_move.from_sq, best_move.to_sq, best_move.prom_p},
        best,
        flag,
        depth
    };
  }
  return;
}

score_t alpha_beta(Board &b, score_t alpha, const score_t beta, const U8 depth,
                   const U8 ply, PVLine *pline)
{
  if (depth == 0) {
    // pline->count = 0; ?
    PVLine line{}; // not used yet
    return quiesce(b, alpha, beta, ply, &line);
  }
  const auto node_hash  = b.t_hash;
  const auto orig_alpha = alpha;
  if (is_repetition(node_hash) || b.hmc == 50) {
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
    PVLine line{};
    movegen_sort(b.stm, std::next(ml.begin(), move_n), sz - move_n,
                 move_n == 0 ? tt_move : TT_move{});
    const Move m = ml[move_n];
    move(b, m);
    if (is_legal(b)) {
      ++node_count;
      score = -alpha_beta(b, -beta, -alpha, depth - 1, ply + 1, &line);
      ++legal_moves;
      if (score > best) {
        best = score;
        if (score > alpha) {
          alpha     = score;
          best_move = m;
          if (b.hmc < 51) {
            (*pline)[0] = m;
            auto it     = std::ranges::find(line, Move{});
            std::copy(line.begin(), it, std::next(pline->begin()));
            if (const auto pit = std::next(pline->begin(),
                                           std::distance(line.begin(), it) + 1);
                pit != pline->end()) {
              *pit = Move{};
            }
          }
        }
      }
      if (score >= beta) {
        if (auto &e = tt[node_hash & tt_mask];
            e.hash != node_hash || e.depth < depth) {
          e = {
              node_hash, {m.from_sq, m.to_sq, m.prom_p},
               score, tt_beta, depth
          };
        }
        if (m.flag != capture && m.flag != prom_capture &&
            m.flag != promotion) {
          auto      &h   = history[~b.stm][m.from_sq][m.to_sq];
          const auto inc = depth * depth;
          h += inc - (h * inc) / hmax;
        }
        unmove(b, m);
        return score;
      }
    }
    unmove(b, m);
    if ((node_count & 127) == 0 && time_up()) {
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
    e = {
        node_hash,
        {best_move.from_sq, best_move.to_sq, best_move.prom_p},
        best,
        flag,
        depth
    };
  }
  return best;
}

score_t quiesce(Board &b, score_t alpha, const score_t beta, const U8 ply,
                PVLine *pline)
{
  PVLine line{};
  if (is_repetition(b.t_hash) || b.hmc == 50) {
    return contempt(b);
  }
  score_t best{};
  if (in_check(b)) {
    best = alpha_beta(b, alpha, beta, 1, ply, &line);
  }
  else {
    best = tmsef(b);
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
    movegen_sort(b.stm, std::next(ml.begin(), move_n), sz - move_n);
    const Move m = ml[move_n];
    if (b.phase != end_game && m.flag != promotion && m.flag != prom_capture &&
        best + piece_val[m.cap_piece] + 200 < alpha) {
      continue;
    }
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
    if ((node_count & 127) == 0 && time_up()) {
      break;
    }
  }
  return best;
}

//------------------------------------------------------------------------------

bool pv_found() { return prev_layer_g_pv[0] != Move{}; }

bool time_up()
{
  time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::steady_clock::now() - start)
                     .count();
  return time_elapsed > allowed_time;
}
