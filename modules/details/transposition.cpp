module;

#include <iostream>
#include <random>

import types;

module transposition;

//------------------------------------------------------------------------------

Zobrist::Zobrist() {
  std::mt19937_64 gen;
  for (long piece_t = 0; piece_t < 6; ++piece_t) {
    for (long color = 0; color < 2; ++color) {
      for (long square = 0; square < 128; ++square) {
        piece_square[piece_t][color][square] = gen();
      }
    }
  }
  stm = gen();
  for (long i = 0; i < 16; ++i) {
    castling[i] = gen();
  }
  for (long i = 0; i < 128; ++i) {
    ep[i] = gen();
  }
}

bool TT_move::operator==(const TT_move &rhs) const {
  return from == rhs.from && to == rhs.to && prom_p == rhs.prom_p;
}

bool TT_move::operator!=(const TT_move &rhs) const { return !(*this == rhs); }

void print_tt_size_info() {
  constexpr auto sz = static_cast<double>(sizeof(TT_entry));
  constexpr auto mb = static_cast<double>(1024 * 1024);
  for (U32 i = 1; i < 33; ++i) {
    const auto n_entries = 1ULL << i;
    std::cout << 2 << " ^ " << i << " = " << n_entries << " entries "
              << static_cast<double>(n_entries) * sz / mb << " MB "
              << std::endl;
  }
}

//------------------------------------------------------------------------------
