module;

#include <array>
#include <chrono>

import attack;
import board;
import config;
import defs;
import eval;
import killer;
import move;
import movegen;
import transposition;

export module search;

//------------------------------------------------------------------------------

export using sctime = std::chrono::time_point<std::chrono::steady_clock>;

export using PVLine = std::array<Move, config::params::max_depth>;

export struct SearchDriver : Killer<config::killer_heuristic> {
  PVLine  pv{};                  ///< populated by alpha_beta_root
  PVLine  prev_pv{};             ///< pv from last finished layer
  score_t eval{};                ///< populated by alpha_beta_root
  score_t prev_eval{};           ///< eval from the last finished layer
  sz_t    root_trees{};          ///< legal moves at root node
  sz_t    root_trees_examined{}; ///< root trees "examined"
  bool    root_beta_cutoff{};    ///< exit alpha_beta_root with beta cutoff
  sctime  start;                 ///< start time point
  long    allowed_time{};
  long    time_elapsed{};
  long    node_count{};
  U8      depth{};

  /**
   * called at the beginning of alpha_beta_root to reset variables for a new
   * layer of iterative deepening
   */
  void new_iteration(Board &b);
};

// todo check elapsed time on node count instead of next move on alpha beta
// maybe that will be less expensive

/**
 * populates the global PV
 */
export void alpha_beta_root(Board &b, score_t alpha, score_t beta,
                            SearchDriver &sd);

/**
 * called by alpha_beta_root to start searching on root nodes
 * @return the best score found by searching to depth `depth`
 */
export score_t alpha_beta(Board &b, score_t alpha, score_t beta, U8 depth,
                          U8 ply, PVLine *pline, SearchDriver &sd);
/**
 * called by alpha_beta to quiet the position before static eval
 * @return static eval from a quiet position
 */
export score_t quiesce(Board &b, score_t alpha, score_t beta, U8 ply,
                       PVLine *pline, SearchDriver &sd);

//------------------------------------------------------------------------------
