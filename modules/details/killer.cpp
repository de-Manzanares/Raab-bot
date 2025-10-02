module;

#include <array>

import config;
import defs;
import move;

module killer;

//------------------------------------------------------------------------------

void Killer<true>::set_killers(const Move &m, const U8 ply)
{
  if (m != killers[ply][0]) {
    for (int i = killer_moves - 1; i > 0; --i) {
      killers[ply][i] = killers[ply][i - 1];
    }
    killers[ply][0] = m;
  }
}

const std::array<Move, killer_moves> &Killer<true>::get_killers(U8 ply) const
{
  return killers[ply];
}

//------------------------------------------------------------------------------

void Killer<false>::set_killers(const Move &m, const U8 ply) {}

auto Killer<false>::get_killers(U8 ply) const -> Killers { return {}; }

//------------------------------------------------------------------------------
