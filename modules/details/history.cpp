module;

#include <array>

import defs;

module history;

namespace raab_bot {

void History<true>::history_decay()
{
  for (int c = 0; c < 2; ++c)
    for (int from = 0; from < 128; ++from)
      for (int to = 0; to < 128; ++to)
        history[c][from][to] >>= 3;
}

I16 History<true>::get_history(const Color color, const Square from, const Square to) const
{
  return history[color][from][to];
}

void History<true>::update_history(const Color color, const Square from, const Square to,
                                   const U8 depth)
{
  const auto h   = get_history(color, from, to);
  const auto inc = depth * depth;
  history[color][from][to] += inc - (h * inc) / hmax;
}

} // namespace raab_bot
