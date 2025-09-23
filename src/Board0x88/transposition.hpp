#ifndef RAAB_BOT_ZOBRIST_HPP
#define RAAB_BOT_ZOBRIST_HPP

#include <cstdint>
#include <random>
#include <unordered_map>

class Zobrist {
 public:
  Zobrist();
  std::uint64_t piece_square[6][2][128];
  std::uint64_t stm{};
  std::uint64_t castling[16];
  std::uint64_t ep[128];
};

inline Zobrist::Zobrist() {
  std::mt19937_64 gen;

  for (int piece_t = 0; piece_t < 6; ++piece_t) {
    for (int color = 0; color < 2; ++color) {
      for (int square = 0; square < 128; ++square) {
        piece_square[piece_t][color][square] = gen();
      }
    }
  }
  stm = gen();
  for (int i = 0; i < 16; ++i) {
    castling[i] = gen();
  }
  for (int i = 0; i < 128; ++i) {
    ep[i] = gen();
  }
}

struct TT_entry {
  int score;
};

/// transposition table
inline std::unordered_map<std::uint64_t, TT_entry> tt;

inline Zobrist zobrist;

#endif // RAAB_BOT_ZOBRIST_HPP
