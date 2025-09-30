/**
 * @file config.cppm
 * toggles for engine features and knobs for engine parameters
 */

module;

export module config;

//------------------------------------------------------------------------------

export namespace config {

constexpr bool delta_pruning = true;

constexpr bool history_heuristic = true;

namespace params {

constexpr int delta = 200;

} // namespace params

} // namespace config

//------------------------------------------------------------------------------
