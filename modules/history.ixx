module;

#include <array>

import defs;

export module history;

namespace raab_bot {

using HistoryArray = std::array<std::array<std::array<I16, 128>, 128>, 2>;

//------------------------------------------------------------------------------

export template <bool> struct History;

//------------------------------------------------------------------------------

template <> struct History<true> {
  HistoryArray      history{};
  void              history_decay();
  [[nodiscard]] I16 get_history(Color color, Square from, Square to) const;
  void              update_history(Color color, Square from, Square to, U8 depth);
};

template <> struct History<false> {
  I16                                   history{};
  static void                           history_decay() {}
  template <typename... Ts> static I16  get_history(Ts... /*unused*/) { return 0; }
  template <typename... Ts> static void update_history(Ts... /*unused*/) {}
};

} // namespace raab_bot
