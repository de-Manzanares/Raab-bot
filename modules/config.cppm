/**
 * @file config.cppm
 * toggles for engine features and knobs for engine parameters
 */

module;

import defs;

export module config;

//------------------------------------------------------------------------------

export namespace config {

constexpr bool delta_pruning = true;

constexpr bool history_heuristic = true;

constexpr bool null_move_pruning = true;

namespace params {

constexpr int delta = 200;

constexpr U8 nmp_reduction = 2;

} // namespace params

} // namespace config

//------------------------------------------------------------------------------
