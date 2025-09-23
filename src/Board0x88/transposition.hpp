#ifndef RAAB_BOT_ZOBRIST_HPP
#define RAAB_BOT_ZOBRIST_HPP

#include <cstdint>
#include <iostream>
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

enum TT_flag : std::uint8_t { tt_exact, tt_alpha, tt_beta };
struct TT_move {
  std::int8_t from = -1;
  std::int8_t to = -1;
  std::uint8_t prom_p = '.';
  bool operator==(const TT_move &rhs) const;
  bool operator!=(const TT_move &rhs) const;
};

inline bool TT_move::operator==(const TT_move &rhs) const {
  return from == rhs.from && to == rhs.to && prom_p == rhs.prom_p;
}

inline bool TT_move::operator!=(const TT_move &rhs) const {
  return !(*this == rhs);
}

struct TT_entry {
  std::uint64_t hash;
  TT_move tt_m;
  int score;
  TT_flag flag;
  std::uint8_t depth;
};

struct TT_probe_ret {
  int score;
  TT_move tt_m;
};

inline void print_tt_size_info() {
  constexpr auto sz = static_cast<double>(sizeof(TT_entry));
  constexpr auto mb = static_cast<double>(1024 * 1024);
  for (std::uint32_t i = 1; i < 33; ++i) {
    const auto n_entries = 1ULL << i;
    std::cout << 2 << " ^ " << i << " = " << n_entries << " entries "
              << static_cast<double>(n_entries) * sz / mb << " MB "
              << std::endl;
  }
}

constexpr std::uint32_t tt_size = (1ULL << 21);
constexpr uint32_t tt_mask = tt_size - 1;

inline TT_entry tt[tt_size];

inline Zobrist zobrist;

#endif // RAAB_BOT_ZOBRIST_HPP
