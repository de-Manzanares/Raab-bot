module;

#include <array>

import defs;

export module history;

using Histories = std::array<std::array<std::array<I16, 128>, 128>, 2>;

//------------------------------------------------------------------------------

export template <bool> struct History;

//------------------------------------------------------------------------------

template <> struct History<true> {
  Histories history{};
  void      history_decay();
  I16       get_history(Color color, Square from, Square to) const;
  void      update_history(Color color, Square from, Square to, U8 depth);
};

template <> struct History<false> {
  I16  history{};
  void history_decay();
  I16  get_history(Color color, Square from, Square to) const;
  void update_history(Color color, Square from, Square to, U8 depth);
};

//------------------------------------------------------------------------------
