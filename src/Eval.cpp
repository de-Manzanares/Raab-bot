/*
 *     ____              __          __          __
 *    / __ \____ _____ _/ /_        / /_  ____  / /_
 *   / /_/ / __ `/ __ `/ __ \______/ __ \/ __ \/ __/
 *  / _, _/ /_/ / /_/ / /_/ /_____/ /_/ / /_/ / /_
 * /_/ |_|\__,_/\__,_/_.___/     /_.___/\____/\__/
 *
 * Copyright (c) 2024 de-Manzanares
 * This work is released under the MIT license.
 *
 */

#include "Eval.h"

#include <ranges>

using s = Square;
using c = Color;

class Node;

bool mat_advantage;

std::unordered_map<char, int> Eval::material_value = {
    {'Q', 900},  {'R', 500},  {'B', 310},  {'N', 300},  {'P', 100},
    {'q', -900}, {'r', -500}, {'b', -310}, {'n', -300}, {'p', -100}};

double Eval::material_evaluation(const Node *n) {
  double sum = 0;
  mat_advantage = false;
  for (auto sq = s::a8; sq >= s::h1; --sq) {
    if (!n->board()->is_empty(sq)) {
      sum += material_value[n->board()->what_piece(sq)];
    }
  }
  if (std::abs(sum) >= 1000) { // if there is a serious material advantage
    mat_advantage = true;
  }
  return sum / 100;
}

int Eval::detect_stalemate_checkmate(const Node *n) {
  uint number_of_moves = 0;

  // count moves, white's turn
  if (n->active_color() == Color::white) {
    for (const auto &moves :
         n->board()->maps->white_moves | std::views::values) {
      number_of_moves += moves.size();
    }
    if (number_of_moves == 0) { // possibly stalemate or checkmate
      if (n->board()->game_state.white_inCheck) { // white is in checkmate
        return -1;
      }
      return 0; // if not checkmate then stalemate
    }
  }
  // count moves, black's turn
  if (n->active_color() == Color::black) {
    for (const auto &moves :
         n->board()->maps->black_moves | std::views::values) {
      number_of_moves += moves.size();
    }
    if (number_of_moves == 0) { // possibly stalemate or checkmate
      if (n->board()->game_state.black_inCheck) { // black is in checkmate
        return 1;
      }
      return 0; // if not checkmate then stalemate
    }
  }
  return 2; // neither stalemate nor checkmate
}

double Eval::mobility_evaluation(const Node *n) {
  double score = 0;
  for (const auto &[sq, moves] : n->board()->maps->white_moves) {
    if (!n->board()->is_white_king(sq)) {
      score += static_cast<double>(moves.size());
    }
  }
  for (const auto &[sq, moves] : n->board()->maps->black_moves) {
    if (!n->board()->is_black_king(sq)) {
      score -= static_cast<double>(moves.size());
    }
  }
  return score * MOBILITY_MULTIPLIER;
}

double Eval::check_bonus(const Node *n) {
  double score = 0;
  if (n->board()->game_state.white_inCheck) {
    if (mat_advantage) {
      score -= CHECK_BONUS / M;
    } else {
      score -= CHECK_BONUS;
    }
  }
  if (n->board()->game_state.black_inCheck) {
    if (mat_advantage) {
      score += CHECK_BONUS / M;
    } else {
      score += CHECK_BONUS;
    }
  }
  return score;
}

double Eval::castle_bonus(const Node *n) {
  double score = 0;

  if ((n->from() == s::e1 && n->to() == s::g1 &&
       n->board()->is_white_rook(s::f1)) ||
      (n->from() == s::e8 && n->to() == s::g8 &&
       n->board()->is_black_rook(s::f8))) {
    score += CASTLE_BONUS;
  }

  else if ((n->from() == s::e1 && n->to() == s::c1 &&
            n->board()->is_black_rook(s::d1)) ||
           (n->from() == s::e8 && n->to() == s::c8 &&
            n->board()->is_black_rook(s::d8))) {
    score -= CASTLE_BONUS;
  }

  return score;
}

double Eval::stacked_pawns(const Node *n) {
  double stacked_balance{};
  std::vector<Square> pawn_w;
  std::vector<Square> pawn_b;
  pawn_w.reserve(8);
  pawn_b.reserve(8);
  std::vector column_w(9, 0);
  std::vector column_b(9, 0);

  for (const auto &sq : n->board()->maps->white_moves | std::views::keys) {
    if (n->board()->is_white_pawn(sq)) {
      pawn_w.push_back(sq);
    }
  }
  for (const auto &p : pawn_w) {
    column_w[n->board()->get_column(p)] += 1;
  }
  for (const auto &c : column_w) {
    if (c > 1) {
      stacked_balance -= c - 1; // bad for white == good for black (-)
    }
  }
  for (const auto &sq : n->board()->maps->black_moves | std::views::keys) {
    if (n->board()->is_black_pawn(sq)) {
      pawn_b.push_back(sq);
    }
  }
  for (const auto &p : pawn_b) {
    column_b[n->board()->get_column(p)] += 1;
  }
  for (const auto &c : column_b) {
    if (c > 1) {
      stacked_balance += c - 1; // bad for black == good for white (+)
    }
  }
  return STACKED_PAWN_PENALTY * stacked_balance;
}

double Eval::passed_pawns(const Node *n) {
  constexpr uint COLUMNS = 8;
  double passed_balance{};
  std::vector<std::vector<Square>> pawn_w;
  std::vector<std::vector<Square>> pawn_b;

  pawn_w.reserve(COLUMNS + 2);
  pawn_b.reserve(COLUMNS + 2);

  for (auto i = 0; i < COLUMNS + 2; i++) {
    std::vector<Square> row{};
    if (i > 0 && i < 9) {
      row.reserve(4);
    }
    pawn_w.push_back(row);
  }
  for (auto i = 0; i < COLUMNS + 2; i++) {
    std::vector<Square> row{};
    if (i > 0 && i < 9) {
      row.reserve(4);
    }
    pawn_b.push_back(row);
  }

  for (const auto &sq : n->board()->maps->white_moves | std::views::keys) {
    if (n->board()->is_white_pawn(sq)) {
      pawn_w[n->board()->get_column(sq)].push_back(sq);
    }
  }
  for (const auto &sq : n->board()->maps->black_moves | std::views::keys) {
    if (n->board()->is_black_pawn(sq)) {
      pawn_b[n->board()->get_column(sq)].push_back(sq);
    }
  }

  for (auto i = 0 + 1; i < COLUMNS + 1; ++i) { // so that [i-1] and [i+1] are ok
    if (pawn_b[i].empty() && !pawn_w[i].empty()) {
      for (const auto &pw : pawn_w[i]) {
        if (pawn_b[i - 1].empty() && pawn_b[i + 1].empty()) {
          passed_balance += 1;
        } else {
          bool passed = true;
          for (std::vector addend{-1, 1}; const auto &a : addend) {
            for (const auto &pb : pawn_b[i + a]) {
              if (n->board()->get_row(pb) > n->board()->get_row(pw)) {
                passed = false;
              }
            }
          }
          if (passed) {
            passed_balance += 1;
          }
        }
      }
    } else if (pawn_w[i].empty() && !pawn_b[i].empty()) {
      for (const auto &pb : pawn_b[i]) {
        if (pawn_w[i - 1].empty() && pawn_w[i + 1].empty()) {
          passed_balance -= 1;
        } else {
          bool passed = true;
          for (std::vector addend{-1, 1}; const auto &a : addend) {
            for (const auto &pw : pawn_w[i + a]) {
              if (n->board()->get_row(pw) < n->board()->get_row(pb)) {
                passed = false;
              }
            }
          }
          if (passed) {
            passed_balance -= 1;
          }
        }
      }
    }
  }
  return PASSED_PAWN_BONUS * passed_balance;
}

double Eval::discourage_early_queen_movement(const Node *n) {
  if (n->board()->game_state.full_move_number <= 10) {
    if (n->active_color() == c::black && n->board()->is_white_queen(n->to())) {
      return -EARLY_QUEEN_PENALTY;
    }
    if (n->active_color() == c::white && n->board()->is_black_queen(n->to())) {
      return EARLY_QUEEN_PENALTY;
    }
  }
  return 0;
}

double Eval::simple_evaluation(const Node *n) {
  if (detect_stalemate_checkmate(n) == -1 || // white is in checkmate
      detect_stalemate_checkmate(n) == 1) {  // black is in checkmate
    return 1000 * detect_stalemate_checkmate(n);
  }
  if (detect_stalemate_checkmate(n) == 0) { // stalemate
    return 0;
  }
  return material_evaluation(n) + mobility_evaluation(n) + check_bonus(n) +
         castle_bonus(n) + discourage_early_queen_movement(n);
}

double Eval::eval(const Node *n) { return simple_evaluation(n); }

// UNUSED
// -----------------------------------------------------------------------------

double Eval::material_ratio(const Board *board) {
  double wscore = 0;
  double bscore = 0;
  for (auto sq = s::a8; sq >= s::h1; --sq) {
    if (!board->is_empty(sq)) {
      if (board->is_white(sq)) {
        wscore += material_value[board->what_piece(sq)];
      } else if (board->is_black(sq)) {
        bscore += material_value[board->what_piece(sq)];
      }
    }
  }
  if (wscore == -bscore) {
    return 0;
  }
  return RATIO_MULTIPLIER * ((wscore + bscore) / (wscore - bscore));
}
