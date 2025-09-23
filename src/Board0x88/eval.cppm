module;

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

enum terminus_t { stalemate, checkmate, null_terminus };

struct Terminus {
  terminus_t term_t;
  int val{};
};

// todo calling movegen twice per search depth seems less than optimal

Terminus terminus_check(Board &b, std::uint8_t ply) {
  Board tmp = b;
  if (cnt_legal_moves(tmp) != 0) {
    return {null_terminus, 0};
  }
  if (in_check(tmp)) {
    return {checkmate, CHECKMATE - ply};
  }
  if (!in_check(tmp)) {
    return {stalemate, 0};
  }
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

score_t attack_enemy_king(Board &b) {
  // get all the squares around the king
  // count the number of them that are under attack
  score_t attack_bonus{};
  Square ksq =
      b.stm == white ? static_cast<Square>(b.wks) : static_cast<Square>(b.bks);
  Color atk_c = b.stm == white ? black : white;
  for (const auto dir : move_vectors[king]) {
    if (const Square to = ksq + dir;
        is_valid_square(to) && is_attacked(b, to, atk_c)) {
      attack_bonus += 1;
    }
  }
  return b.stm == white ? -attack_bonus : attack_bonus;
}

// kinda sorta static, we still use movegen to count legal moves for
// terminal detection
export constexpr score_t static_eval(Board &b, std::uint8_t ply) {
  // doesn't find checkmate or stalemate
  switch (auto [term_t, term_v] = terminus_check(b, ply); term_t) {
  case checkmate:
    return -term_v;
  case stalemate:
    return term_v;
  default:;
  }
  const auto score = material(b) * 128 + mobility(b) / 8 + check_bonus(b) +
                     attack_enemy_king(b) * 16;
  return score * (b.stm == white ? 1 : -1);
}
