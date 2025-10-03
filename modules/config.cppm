/**
 * @file config.cppm
 * toggles for engine features and knobs for engine parameters
 */

module;

import defs;

export module config;

//------------------------------------------------------------------------------

export namespace config {

// pruning
constexpr bool delta_pruning     = true;
constexpr bool null_move_pruning = true;

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

using namespace config;
using namespace config::params;

// these don't seem to mix well in my engine ... yet
static_assert(history_heuristic != killer_heuristic);

static_assert(killer_heuristic == false || killer_moves > 0);
