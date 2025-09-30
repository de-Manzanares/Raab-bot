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

bool time_up(SearchDriver &sd);

//------------------------------------------------------------------------------

void alpha_beta_root(Board &b, score_t alpha, score_t beta, SearchDriver &sd)
{
  // todo encapsulate into sd.start_new_iteration kinda function
  std::ranges::fill(sd.pv, Move{});
  sd.eval             = 0;
  sd.rte              = 0;
  sd.root_beta_cutoff = false;
  // todo small optimization don't need to call movegen twice here
  sd.root_trees = cnt_legal_moves(b);
  sd.node_count = 0;

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

  for (const auto sz = movegen(b, ml.begin()); move_n < sz;
       ++move_n, ++sd.rte) {
    PVLine line{};
    movegen_sort(b.stm, std::next(ml.begin(), move_n), sz - move_n,
                 move_n == 0 ? tt_move : TT_move{});
    const Move m = ml[move_n];
    move(b, m);
    if (is_legal(b)) {
      ++sd.node_count;
      constexpr U8 ply{};
      sd.eval = -alpha_beta(b, -beta, -alpha, sd.depth - 1, ply + 1, &line, sd);
      ++legal_moves;
      if (sd.eval > best) {
        best      = sd.eval;
        best_move = m;
        if (sd.eval > alpha) {
          alpha    = sd.eval;
          sd.pv[0] = m;
          auto it  = std::ranges::find(line, Move{});
          std::copy(line.begin(), it, std::next(sd.pv.begin()));
          if (const auto pit =
                  std::next(sd.pv.begin(), std::distance(line.begin(), it) + 1);
              pit != sd.pv.end()) {
            *pit = Move{};
          }
        }
      }
      if (sd.eval >= beta) {
        if (auto &e = tt[node_hash & tt_mask];
            e.hash != node_hash || e.depth < sd.depth) {
          e = {
              node_hash,
              {m.from_sq, m.to_sq, m.prom_p},
              sd.eval,
              tt_beta,
              sd.depth
          };
        }
        if (m.flag != capture && m.flag != prom_capture &&
            m.flag != promotion) {
          auto      &h   = b.history[~b.stm][m.from_sq][m.to_sq];
          const auto inc = sd.depth * sd.depth;
          h += inc - (h * inc) / hmax;
        }
        unmove(b, m);
        sd.root_beta_cutoff = true;
        return;
      }
    }
    unmove(b, m);
    if ((sd.node_count & 127) == 0 && time_up(sd)) {
      if (sd.prev_pv[0] == Move{}) {
        sd.prev_pv[0] = best_move;
        sd.prev_pv[1] = Move{};
      }
      break;
    }
  }
  if (legal_moves == 0) {
    return;
  }
  const TT_flag flag = (best <= orig_alpha) ? tt_alpha : tt_exact;
  if (auto &e = tt[node_hash & tt_mask];
      e.hash != node_hash || e.depth < sd.depth) {
    e = {
        node_hash,
        {best_move.from_sq, best_move.to_sq, best_move.prom_p},
        best,
        flag,
        sd.depth
    };
  }
  return;
}

score_t alpha_beta(Board &b, score_t alpha, const score_t beta, const U8 depth,
                   const U8 ply, PVLine *pline, SearchDriver &sd)
{
  if (depth == 0) {
    // pline->count = 0; ?
    PVLine line{}; // not used yet
    return quiesce(b, alpha, beta, ply, &line, sd);
  }
  const auto node_hash  = b.t_hash;
  const auto orig_alpha = alpha;
  if (is_repetition(b) || b.hmc == 50) {
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
      ++sd.node_count;
      score = -alpha_beta(b, -beta, -alpha, depth - 1, ply + 1, &line, sd);
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
          auto      &h   = b.history[~b.stm][m.from_sq][m.to_sq];
          const auto inc = depth * depth;
          h += inc - (h * inc) / hmax;
        }
        unmove(b, m);
        return score;
      }
    }
    unmove(b, m);
    if ((sd.node_count & 127) == 0 && time_up(sd)) {
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
                PVLine *pline, SearchDriver &sd)
{
  PVLine line{};
  if (is_repetition(b) || b.hmc == 50) {
    return contempt(b);
  }
  score_t best{};
  if (in_check(b)) {
    best = alpha_beta(b, alpha, beta, 1, ply, &line, sd);
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
      ++sd.node_count;
      score = -quiesce(b, -beta, -alpha, ply + 1, pline, sd);
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
    if ((sd.node_count & 127) == 0 && time_up(sd)) {
      break;
    }
  }
  return best;
}

//------------------------------------------------------------------------------

bool time_up(SearchDriver &sd)
{
  sd.time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::steady_clock::now() - sd.start)
                        .count();
  return sd.time_elapsed > sd.allowed_time;
}
