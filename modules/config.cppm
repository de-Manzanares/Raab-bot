/**
 * @file config.cppm
 * toggles for engine features and knobs for engine parameters
 */

module;

import defs;

export module config;

namespace raab_bot {

//------------------------------------------------------------------------------

export namespace config {

// pruning
constexpr bool delta_pruning     = true;
constexpr bool futility_pruning  = false;
constexpr bool null_move_pruning = true;
constexpr bool razoring          = false;

// move ordering
constexpr bool history_heuristic = true;
constexpr bool killer_heuristic  = false;

namespace params {

constexpr int delta         = 200;
constexpr I8  killer_moves  = killer_heuristic ? 2 : 0;
constexpr U8  nmp_reduction = 2;
constexpr U8  max_depth     = 32;

} // namespace params

} // namespace config

//------------------------------------------------------------------------------

using config::history_heuristic;
using config::killer_heuristic;
using config::params::killer_moves;

// these don't seem to mix well in my engine ... yet
static_assert(history_heuristic != killer_heuristic);

static_assert(killer_heuristic == false || killer_moves > 0);

} // namespace raab_bot
