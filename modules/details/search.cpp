module;

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <limits>
#include <ranges>

import attack;
import board;
import config;
import eval;
import move;
import movegen;
import transposition;
import defs;

module search;

void bubble_up_pv(PVLine &p_pv, const Move &best_move, const PVLine &c_pv);
bool time_up(SearchDriver &sd);
bool check_time(SearchDriver &sd);
bool is_draw(const Board &b);
bool is_quiet(const Move &m);
void tt_entry(U64 hash, const Move &m, score_t eval, TT_flag flag, U8 depth);

//------------------------------------------------------------------------------

void alpha_beta_root(Board &b, score_t alpha, score_t beta, SearchDriver &sd)
{
  sd.new_iteration(b);

  const auto node_hash  = b.t_hash;
  const auto orig_alpha = alpha;

  // probe TT
  TT_move tt_move{};
  if (const auto &e = tt[node_hash & tt_mask]; e.hash == node_hash) {
    tt_move = e.tt_m;
  }

  score_t  best_eval = std::numeric_limits<score_t>::min();
  Move     best_move{};
  sz_t     legal_moves{};
  MoveList ml{};
  sz_t     move_n{};

  for (const auto sz = movegen(b, ml.begin()); move_n < sz;
       ++move_n, ++sd.root_trees_examined) {

    const auto first_unseen = std::next(ml.begin(), move_n);
    const auto ml_end       = sz - move_n;

    // use the transposition table only for the first root tree
    // otherwise, allow move ordering to do its work
    if (move_n == 0) {
      move_select(first_unseen, ml_end, tt_move);
    }
    else {
      move_select(first_unseen, ml_end);
    }

    const Move m = ml[move_n];
    move(b, m);

    if (!is_legal(b)) {
      unmove(b, m);
      continue;
    }

    PVLine       line{};
    constexpr U8 ply{};

    ++legal_moves;
    ++sd.node_count;

    sd.eval = -alpha_beta(b, -beta, -alpha, sd.depth - 1, ply + 1, &line, sd);

    if (sd.eval > best_eval) {
      best_eval = sd.eval;
      best_move = m;
      if (sd.eval > alpha) {
        alpha = sd.eval;
        bubble_up_pv(sd.pv, best_move, line);
      }
    }

    if (sd.eval >= beta) {
      tt_entry(node_hash, m, sd.eval, tt_beta, sd.depth);
      if constexpr (config::history_heuristic) {
        if (is_quiet(m)) {
          b.update_history(~b.stm, m.from_sq, m.to_sq, sd.depth);
        }
      }
      unmove(b, m);
      sd.root_beta_cutoff = true;
      return;
    }

    unmove(b, m);

    if (time_up(sd)) {

      // not sure if this is the way I want to do this ...
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

  const TT_flag flag = (best_eval <= orig_alpha) ? tt_alpha : tt_exact;
  tt_entry(node_hash, best_move, best_eval, flag, sd.depth);
}

score_t alpha_beta(Board &b, score_t alpha, const score_t beta, const U8 depth,
                   const U8 ply, PVLine *pline, SearchDriver &sd)
{
  if (depth == 0) {
    PVLine line{};
    return quiesce(b, alpha, beta, ply, &line, sd);
  }

  if (is_draw(b)) {
    const auto eval = contempt(b);
    tt_entry(b.t_hash, {}, eval, tt_exact, depth);
    return eval;
  }

  const auto node_hash  = b.t_hash;
  const auto orig_alpha = alpha;

  // probe tt
  TT_move tt_move{};
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

  score_t  best_eval = std::numeric_limits<score_t>::min();
  Move     best_move{};
  sz_t     legal_moves{};
  MoveList ml{};
  sz_t     move_n{};
  score_t  eval{};

  for (const auto sz = movegen(b, ml.begin()); move_n < sz; ++move_n) {

    const auto first_unseen = std::next(ml.begin(), move_n);
    const auto ml_end       = sz - move_n;

    // use the transposition table only for the first root tree
    // otherwise, allow move ordering to do its work
    if (move_n == 0) {
      move_select(first_unseen, ml_end, tt_move);
    }
    else {
      move_select(first_unseen, ml_end);
    }

    const Move m = ml[move_n];
    move(b, m);

    if (!is_legal(b)) {
      unmove(b, m);
      continue;
    }

    ++legal_moves;
    ++sd.node_count;

    PVLine line{};
    eval = -alpha_beta(b, -beta, -alpha, depth - 1, ply + 1, &line, sd);

    if (eval > best_eval) {
      best_eval = eval;
      best_move = m;
      if (eval > alpha) {
        alpha = eval;
        if (b.hmc < 51) {
          bubble_up_pv(*pline, best_move, line);
        }
      }
    }

    if (eval >= beta) {
      tt_entry(node_hash, m, eval, tt_beta, depth);
      if constexpr (config::history_heuristic) {
        if (is_quiet(m)) {
          b.update_history(~b.stm, m.from_sq, m.to_sq, sd.depth);
        }
      }
      unmove(b, m);
      return eval;
    }

    unmove(b, m);

    if (time_up(sd)) {
      break;
    }
  }

  if (legal_moves == 0) {
    const score_t terminal_eval = in_check(b) ? -(CHECKMATE - ply) : 0;
    tt_entry(node_hash, {}, terminal_eval, tt_exact, depth);
    return terminal_eval;
  }

  const TT_flag flag = (best_eval <= orig_alpha) ? tt_alpha : tt_exact;
  tt_entry(node_hash, best_move, best_eval, flag, depth);
  return best_eval;
}

score_t quiesce(Board &b, score_t alpha, const score_t beta, const U8 ply,
                PVLine *pline, SearchDriver &sd)
{
  if (is_draw(b)) {
    return contempt(b);
  }

  score_t best_eval{};

  if (in_check(b)) {
    PVLine line{};
    best_eval = alpha_beta(b, alpha, beta, 1, ply, &line, sd);
  }
  else {
    best_eval = tmsef(b);
  }
  if (best_eval >= beta) {
    return best_eval;
  }
  alpha = std::max(best_eval, alpha);

  score_t  eval{};
  MoveList ml{};
  sz_t     move_n{};

  for (const auto sz = quiescence_movegen(b, ml.begin()); move_n < sz;
       ++move_n) {

    move_select(std::next(ml.begin(), move_n), sz - move_n);
    const Move m = ml[move_n];

    if constexpr (config::delta_pruning) {
      if (b.phase != end_game && m.flag != promotion &&
          m.flag != prom_capture &&
          best_eval + piece_val[m.cap_piece] + config::params::delta < alpha) {
        continue;
      }
    }

    move(b, m);
    if (!is_legal(b)) {
      unmove(b, m);
      continue;
    }

    ++sd.node_count;
    eval = -quiesce(b, -beta, -alpha, ply + 1, pline, sd);

    if (eval > best_eval) {
      best_eval = eval;
      if (eval > alpha) {
        PVLine line{};
        alpha = eval;
        if (b.hmc < 51) {
          bubble_up_pv(*pline, m, line);
        }
      }
    }

    if (eval >= beta) {
      unmove(b, m);
      return eval;
    }

    unmove(b, m);
    if (time_up(sd)) {
      break;
    }
  }
  return best_eval;
}

//------------------------------------------------------------------------------

void SearchDriver::new_iteration(Board &b)
{
  std::ranges::fill(pv, Move{});
  eval                = 0;
  root_trees_examined = 0;
  root_beta_cutoff    = false;
  // clang-format off
  // todo small optimization don't need to call movegen twice per alpha_beta_root
  // clang-format on
  root_trees = cnt_legal_moves(b);
  node_count = 0;
}

void bubble_up_pv(PVLine &p_pv, const Move &best_move, const PVLine &c_pv)
{
  p_pv[0] = best_move;
  const auto child_line =
      c_pv | std::views::take_while([](const Move &m) { return m != Move{}; });
  const auto [in, out] = std::ranges::copy(child_line, std::next(p_pv.begin()));
  if (out != p_pv.end()) {
    *out = Move{}; // sentinel
  }
}

bool time_up(SearchDriver &sd)
{
  constexpr U16 check_every_n_nodes = (1UL << 11UL) - 1;
  if ((sd.node_count & check_every_n_nodes) == 0) {
    sd.time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                          std::chrono::steady_clock::now() - sd.start)
                          .count();
    return sd.time_elapsed > sd.allowed_time;
  }
  return false;
}

bool is_draw(const Board &b) { return b.hmc > 49 || is_repetition(b); }

bool is_quiet(const Move &m)
{
  return m.flag != capture && m.flag != prom_capture && m.flag != promotion;
}

void tt_entry(const U64 hash, const Move &m, const score_t eval,
              const TT_flag flag, const U8 depth)
{
  if (auto &e = tt[hash & tt_mask]; e.hash != hash || e.depth < depth) {
    e = {
        hash, {m.from_sq, m.to_sq, m.prom_p},
         eval, flag, depth,
    };
  }
}
