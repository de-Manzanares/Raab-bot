#include "Eval.h"

#include <ranges>

using s = Square;

struct Node;

double Eval::CHECK_BONUS = 0.25;
double Eval::MOBILITY_MULTIPLIER = 1.0 / 100.0;
double Eval::CASTLE_BONUS = 0.5;
double Eval::RATIO_MULTIPLIER = 2.0;

std::unordered_map<char, int> Eval::material_value = {
    {'Q', 900},  {'R', 500},  {'B', 310},  {'N', 300},  {'P', 100},
    {'q', -900}, {'r', -500}, {'b', -310}, {'n', -300}, {'p', -100}};

double Eval::material_evaluation(const Node *n) {
  double sum = 0;
  for (auto sq = s::a8; sq >= s::h1; --sq) {
    if (!n->_board->is_empty(sq)) {
      sum += material_value[n->_board->what_piece(sq)];
    }
  }
  return sum / 100;
}

int Eval::detect_stalemate_checkmate(const Node *n) {
  uint number_of_moves = 0;

  // count moves, white's turn
  if (n->_board->game_state.active_color == Color::white) {
    for (const auto &moves :
         n->_board->maps->white_moves | std::views::values) {
      number_of_moves += moves.size();
    }
    if (number_of_moves == 0) { // possibly stalemate or checkmate
      if (n->_board->game_state.white_inCheck) { // white is in checkmate
        return -1;
      }
      return 0; // if not checkmate then stalemate
    }
  }
  // count moves, black's turn
  if (n->_board->game_state.active_color == Color::black) {
    for (const auto &moves :
         n->_board->maps->black_moves | std::views::values) {
      number_of_moves += moves.size();
    }
    if (number_of_moves == 0) { // possibly stalemate or checkmate
      if (n->_board->game_state.black_inCheck) { // black is in checkmate
        return 1;
      }
      return 0; // if not checkmate then stalemate
    }
  }
  return 2; // neither stalemate nor checkmate
}

double Eval::mobility_evaluation(const Node *n) {
  double score = 0;
  for (const auto &moves : n->_board->maps->white_moves | std::views::values) {
    score += static_cast<double>(moves.size());
  }
  for (const auto &moves : n->_board->maps->black_moves | std::views::values) {
    score -= static_cast<double>(moves.size());
  }
  return score * MOBILITY_MULTIPLIER;
}

double Eval::check_bonus(const Node *n) {
  double score = 0;
  if (n->_board->game_state.white_inCheck) {
    score -= CHECK_BONUS;
  }
  if (n->_board->game_state.black_inCheck) {
    score += CHECK_BONUS;
  }
  return score;
}

double Eval::castle_bonus(const Node *n) {
  double score = 0;

  if ((n->_from == s::e1 && n->_to == s::g1 &&
       n->_board->is_white_rook(s::f1)) ||
      (n->_from == s::e8 && n->_to == s::g8 &&
       n->_board->is_black_rook(s::f8))) {
    score += CASTLE_BONUS;
  }

  else if ((n->_from == s::e1 && n->_to == s::c1 &&
            n->_board->is_black_rook(s::d1)) ||
           (n->_from == s::e8 && n->_to == s::c8 &&
            n->_board->is_black_rook(s::d8))) {
    score -= CASTLE_BONUS;
  }

  return score;
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
         castle_bonus(n);
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

double Eval::discourage_early_queen_movement(const Node *node) {
  double score = 0;
  if (node->_board->game_state.full_move_number <= 10) {
    if (node->parent != nullptr) {
      if (node->parent->_board->is_queen(node->_from)) {
        if (node->parent->_board->game_state.active_color == Color::white) {
          score -= .9;
        }
        if (node->parent->_board->game_state.active_color == Color::black) {
          score += .9;
        }
      } else if (!node->parent->_board->is_pawn(node->_from)) {
        if (node->parent->_board->game_state.active_color == Color::white) {
          score += .5;
        }
        if (node->parent->_board->game_state.active_color == Color::black) {
          score -= .5;
        }
      }
    }
  }
  return score;
}
