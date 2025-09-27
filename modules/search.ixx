module;

#include <array>
#include <chrono>

import attack;
import board;
import eval;
import move;
import movegen;
import transposition;
import types;

export module search;

export using sctime = std::chrono::time_point<std::chrono::steady_clock>;

//------------------------------------------------------------------------------

export constexpr U8 max_depth = 32;

export using PVLine = std::array<Move, max_depth>;

export PVLine g_pv;      ///< populated by alpha_beta_root
export PVLine prev_g_pv; ///< copies g_pv each time a full layer is searched

export score_t g_eval; ///< populated by alpha_beta_root
export score_t prev_g_eval;

export sz_t root_trees{};             ///< legal moves at root node
export sz_t rte{};                    ///< root trees "examined"
export bool root_beta_cutoff = false; ///< exit alpha_beta_root with beta cutoff

export sctime start;
export long   allowed_time;
export long   time_elapsed;
export long   node_count;
export U8     depth;

// todo check elapsed time on node count instead of next move on alpha beta
// maybe that will be less expensive

/**
 * populates the global PV
 */
export void alpha_beta_root(Board &b, score_t alpha, score_t beta, U8 depth);

/**
 * called by alpha_beta_root to start searching on root nodes
 * @return the best score found by searching to depth `depth`
 */
export score_t alpha_beta(Board &b, score_t alpha, score_t beta, U8 depth,
                          U8 ply, PVLine *pline);
/**
 * called by alpha_beta to quiet the position before static eval
 * @return static eval from a quiet position
 */
export score_t quiesce(Board &b, score_t alpha, score_t beta, U8 ply,
                       PVLine *pline);

//------------------------------------------------------------------------------
