module;

#include <iostream>
#include <random>

import defs;

module transposition;

//------------------------------------------------------------------------------

Zobrist::Zobrist()
{
  std::mt19937_64 gen;
  for (long piece_t = 0; piece_t < 6; ++piece_t) {
    for (long color = 0; color < 2; ++color) {
      for (long square = 0; square < 128; ++square) {
        pcs[piece_t][color][square] = gen();
      }
    }
  }
  stm = gen();
  for (long i = 0; i < 16; ++i) {
    cr[i] = gen();
  }
  for (long i = 0; i < 128; ++i) {
    ep[i] = gen();
  }
  for (long piece_t = 0; piece_t < 6; ++piece_t) {
    for (long color = 0; color < 2; ++color) {
      mat[piece_t][color] = gen();
    }
  }
}

bool TT_move::operator==(const TT_move &rhs) const
{
  return from == rhs.from && to == rhs.to && prom_p == rhs.prom_p;
}

bool TT_move::operator!=(const TT_move &rhs) const { return !(*this == rhs); }

//------------------------------------------------------------------------------
