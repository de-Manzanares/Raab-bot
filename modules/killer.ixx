module;

#include <array>

import config;
import defs;
import move;

export module killer;

using namespace config;
using namespace config::params;

using Killers    = std::array<Move, killer_moves>;
using KillerList = std::array<Killers, 2 * max_depth>;

//------------------------------------------------------------------------------

export template <bool> struct Killer;

//------------------------------------------------------------------------------

template <> struct Killer<true> {
  KillerList killers{};
  void       set_killers(const Move &m, U8 ply);
  auto       get_killers(U8 ply) const -> const Killers &;
};

template <> struct Killer<false> {
  void set_killers(const Move &m, U8 ply);
  auto get_killers(U8 ply) const -> Killers;
};

//------------------------------------------------------------------------------
