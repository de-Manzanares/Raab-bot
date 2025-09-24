module;

#include "transposition.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <limits>

export module Board0x88:eval;
import :core;
import :move;
import :movegen;

export using score_t = std::int32_t;

// arbitrary values should be powers of 2 from now on LOL

constexpr score_t CHECKMATE = 1ULL << 30;

constexpr int piece_vals[6] = {0, 9, 5, 3, 3, 1};

bool in_check(const Board &b) {
  const Square sq =
      b.stm == white ? static_cast<Square>(b.wks) : static_cast<Square>(b.bks);
  return is_attacked(b, sq, ~b.stm);
}

constexpr score_t material(const Board &b) {
  int mat{};
  for (const auto sq : square_sequence) {
    if (b.piece_on[sq] != null_piece) {
      auto [piece_t, color] = b.piece_info(sq);
      const int mult = color == white ? 1 : -1;
      mat += piece_vals[piece_t] * mult;
    }
  }
  return mat;
}

// todo remove uses of cnt_legal_moves();
score_t mobility(const Board &b) {
  Board tmp = b;
  score_t w_mob{};
  score_t b_mob{};
  tmp.stm = white;
  w_mob = cnt_legal_moves(tmp);
  tmp.stm = black;
  b_mob = cnt_legal_moves(tmp);
  return w_mob - b_mob;
}

score_t check_bonus(Board &b) {
  score_t check_bonus{};
  if (b.stm == white) {
    if (is_attacked(b, static_cast<Square>(b.wks), black)) {
      check_bonus = -16;
    }
  } else {
    if (is_attacked(b, static_cast<Square>(b.bks), white)) {
      check_bonus = 16;
    }
  }
  return check_bonus;
}

score_t king_restriction(Board &b) {
  // get all the squares around the king
  // count the number of them that are under attack
  // ratio of attacked to valid squares
  auto ratio = [&b](const Square ksq, const Color c) {
    double att{};
    score_t valid{};
    for (const auto dir : move_vectors[king]) {
      if (is_valid_square(ksq + dir)) {
        ++valid;
      }
      if (is_attacked(b, ksq + dir, c)) {
        ++att;
      }
    }
    return att / valid;
  };
  const auto w = ratio(static_cast<Square>(b.wks), black);
  const auto bl = ratio(static_cast<Square>(b.bks), white);
  return w - bl;
}

export constexpr score_t static_eval(Board &b, int alpha, int beta,
                                     std::uint8_t ply) {
  // todo dedicated eval cache

  auto score = (material(b) * 128) + (mobility(b) >> 3) + check_bonus(b) -
               (king_restriction(b) << 3);
  score *= (b.stm == white ? 1 : -1);
  return score;
}

export score_t contempt(const Board &b) {
  return (material(b) * (b.stm == white ? -1 : 1) * 128);
}
