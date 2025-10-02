module;

#include <array>

import defs;

export module history;

//------------------------------------------------------------------------------

export template <bool> struct History;

//------------------------------------------------------------------------------

template <> struct History<true> {
  std::array<std::array<std::array<I16, 128>, 128>, 2> history{};
  void history_decay();
  I16  get_history(Color color, Square from, Square to) const;
  I16 &get_history(Color color, Square from, Square to);
  void update_history(Color color, Square from, Square to, U8 depth);
};

template <> struct History<false> {
  I16         history{};
  static void history_decay();
  static I16  get_history(Color color, Square from, Square to);
  static void update_history(Color color, Square from, Square to, U8 depth);
};

void History<true>::history_decay()
{
  for (int c = 0; c < 2; ++c)
    for (int from = 0; from < 128; ++from)
      for (int to = 0; to < 128; ++to)
        history[c][from][to] >>= 3;
}

I16 History<true>::get_history(const Color color, const Square from,
                               const Square to) const
{
  return history[color][from][to];
}

I16 &History<true>::get_history(const Color color, const Square from,
                                const Square to)
{
  return history[color][from][to];
}

void History<true>::update_history(const Color color, const Square from,
                                   const Square to, const U8 depth)
{
  auto      &h   = get_history(color, from, to);
  const auto inc = depth * depth;
  h += inc - (h * inc) / hmax;
}

//------------------------------------------------------------------------------

void History<false>::history_decay() {}

I16 History<false>::get_history(const Color color, const Square from,
                                const Square to)
{
  return 0;
}

void History<false>::update_history(const Color color, const Square from,
                                    const Square to, const U8 depth)
{
}

//------------------------------------------------------------------------------
