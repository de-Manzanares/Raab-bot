#include "Board.h"
#include "Node.h"

#include <Eval.h>
#include <UCI.h>
#include <catch2/catch_all.hpp>
#include <iostream>

using s = Square;

struct Move {
  Square from{};
  Square to{};
  char promotion{};
  bool operator!=(const Move &rhs) const;
  friend std::ostream &operator<<(std::ostream &out, const Move &m);
};

std::ostream &operator<<(std::ostream &out, const Move &m) {
  out << Sq::square_to_string(m.from) << Sq::square_to_string(m.to);
  if (m.promotion != 0) {
    out << m.promotion;
  }
  return out;
}

bool Move::operator!=(const Move &rhs) const {
  return !(from == rhs.from && to == rhs.to && promotion == rhs.promotion);
}

std::vector<Move> movelist;

double implied_tree(Board &b, const uint depth, double alpha, // NOLINT
                    double beta, const bool maxing, std::vector<Move> &movelist,
                    const Square from = s::h1, const Square to = s::h1,
                    const char ch = 0) {

  if (depth == 0) {
    return Eval::eval(&b, from, to, 0);
  }
  if (Eval::detect_stalemate_checkmate(&b) != 2) {
    return Eval::eval(&b, from, to, 0);
  }
  Counter::node = 0;
  double best_eval;

  std::stack<Game_State> gss;
  b.update_move_maps();
  auto [white_influence, black_influence, white_pinned, black_pinned,
        white_moves, black_moves] = b.maps;
  std::vector<Move> inner_movelist;
  inner_movelist.reserve(8);

  if (maxing) {
    best_eval = -100'000;
    for (const auto &[sq, moves] : white_moves) {
      for (const auto &move : moves) {
        gss.push(b.game_state);
        b.do_move(sq, move, 0);
        Counter::node++;

        const auto res = implied_tree(b, depth - 1, alpha, beta, false,
                                      movelist, sq, move, 0);

        b.undo_move(sq, move, 0);
        b.game_state = gss.top();
        gss.pop();

        if (best_eval < res) {
          best_eval = res;
          inner_movelist.push_back({sq, move, ch});
        }
        alpha = std::max(alpha, res);
        if (beta <= alpha) {
          break;
        }
      }
    }
  } else {
    best_eval = 100'000;
    for (const auto &[sq, moves] : black_moves) {
      for (const auto &move : moves) {
        gss.push(b.game_state);
        b.do_move(sq, move, 0);

        const auto res = implied_tree(b, depth - 1, alpha, beta, true, movelist,
                                      sq, move, 0);

        b.undo_move(sq, move, 0);
        b.game_state = gss.top();
        gss.pop();

        if (best_eval > res) {
          best_eval = res;
          inner_movelist.push_back({sq, move, ch});
        }
        beta = std::min(alpha, res);
        if (beta <= alpha) {
          break;
        }
      }
    }
  }
  if (movelist.empty() || movelist.back() != inner_movelist.back()) {
    movelist.push_back(inner_movelist.back());
  }
  return best_eval;
}

void print(std::vector<std::pair<Square, Square>> &movelist) {
  for (const auto &[fst, snd] : movelist) {
    std::cout << Sq::square_to_string(fst) << Sq::square_to_string(snd);
    std::cout << " ";
  }
  std::cout << "\n";
}

void test_itt(Board &b, const uint depth, const std::string &fen) {
  b.import_fen(fen);
  std::cout << implied_tree(b, depth, -100'000, 100'000, true, movelist)
            << "\t";
  // for (const auto &m : movelist) {
  //   std::cout << m << " ";
  // }
  std::cout << "\n";
}

TEST_CASE("bruh") {
  SECTION("mate in 0.5") {
    Board b;
    test_itt(b, 1, "5k2/1R6/5K2/8/8/8/8/8 w - - 0 1");
  }
  SECTION("mate in 1.5") {
    Board b;
    movelist.clear();
    test_itt(b, 3, "8/5k2/2R5/1R3K2/8/8/8/8 w - - 0 1");
  }
  SECTION("mate in 2.5") {
    Board b;
    movelist.clear();
    test_itt(b, 5, "8/8/5k2/1R6/2R2K2/8/8/8 w - - 0 1");
  }
}
