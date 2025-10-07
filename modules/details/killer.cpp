module;

import Array;
import config;
import defs;
import move;

module killer;

namespace raab_bot {

void Killer<true>::set_killers(const Move &m, const U8 ply)
{
  if (m != killers[ply][0]) {
    for (int i = killer_moves - 1; i > 0; --i) {
      killers[ply][i] = killers[ply][i - 1];
    }
    killers[ply][0] = m;
  }
}

const Array<Move, killer_moves> &Killer<true>::get_killers(const U8 ply) const
{
  return killers[ply];
}

} // namespace raab_bot
