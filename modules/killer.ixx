module;

#include <array>

import config;
import defs;
import move;

export module killer;

namespace raab_bot {

using config::params::killer_moves;
using config::params::max_depth;

using Killers    = std::array<Move, killer_moves>;
using KillerList = std::array<Killers, 2 * max_depth>;

//------------------------------------------------------------------------------

export template <bool> struct Killer;

//------------------------------------------------------------------------------

template <> struct Killer<true> {
  KillerList         killers{};
  void               set_killers(const Move &m, U8 ply);
  [[nodiscard]] auto get_killers(U8 ply) const -> const Killers &;
};

template <> struct Killer<false> {
  static void set_killers(const Move &m, U8 ply);
  static auto get_killers(U8 ply) -> Killers; // NOLINT(*-use-nodiscard)
};

//------------------------------------------------------------------------------

} // namespace raab_bot
