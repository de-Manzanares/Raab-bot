#include "../include/Eval.h"

using s = Square;

struct Node;

double Eval::CHECK_BONUS = 0.25;
double Eval::MOBILITY_MULTIPLIER = 1.0 / 100.0;
double Eval::CASTLE_BONUS = 0.5;
double Eval::RATIO_MULTIPLIER = 2.0;

/// material value
std::unordered_map<char, int> Eval::material_value = {
    {'Q', 900},  {'R', 500},  {'B', 310},  {'N', 300},  {'P', 100},
    {'q', -900}, {'r', -500}, {'b', -310}, {'n', -300}, {'p', -100}};

/**
 * @brief Evaluates the material value on the given chess board.
 * @details This function calculates the material value of the chess pieces on
 * the given chess board.
 * @param n A pointer to the chess board to be evaluated.
 * @return The material value of the chess pieces on the board divided by 100.
 */
double Eval::material_evaluation(const Node *n) {
  double sum = 0;
  for (Square sq = s::a8; sq >= s::h1; --sq) {
    if (!n->_board->is_empty(sq)) {
      sum += material_value[n->_board->what_piece(sq)];
    }
  }
  return sum / 100;
}

/**
 * @brief Detects if the given chess board is in a checkmate position.
 * @details Check whether there are any legal moves available for the active
 * player. If there are no legal moves available, this is checkmate.
 * @param n The chess board for which to detect checkmate.
 * @return -1 if White is in checkmate, 1 if Black is in checkmate, 0 if neither
 * color is in checkmate.
 */
int Eval::detect_checkmate(const Node *n) {
  ulong number_of_moves = 0;

  // detect white checkmate
  if (n->_board->game_state.active_color == Color::white) {
    if (n->_board->game_state.white_inCheck) {
      for (const auto &[sq, moves] : n->_board->maps->white_moves) {
        number_of_moves += moves.size();
      }
      if (number_of_moves == 0) {
        return -1;
      }
    }
  }
  // detect black checkmate
  if (n->_board->game_state.active_color == Color::black) {
    if (n->_board->game_state.black_inCheck) {
      for (const auto &[sq, moves] : n->_board->maps->black_moves) {
        number_of_moves += moves.size();
      }
      if (number_of_moves == 0) {
        return 1;
      }
    }
  }
  // if neither is in checkmate
  return 0;
}

/**
 * @brief Evaluates the mobility of a given chess board.
 * @details Adds score for each legal move available to a side.
 * @param n A pointer to the chess board.
 * @return The mobility score.
 */
double Eval::mobility_evaluation(const Node *n) {
  double score = 0;
  for (const auto &[sq, moves] : n->_board->maps->white_moves) {
    score += static_cast<double>(moves.size());
  }
  for (const auto &[sq, moves] : n->_board->maps->black_moves) {
    score -= static_cast<double>(moves.size());
  }
  return score * MOBILITY_MULTIPLIER;
}

/**
 * Calculates the bonus score for a given board based on whether a player is in
 * check.
 * @param n The board to check for in-check status.
 * @return (+) score is black is in check, (-) score if white is in check
 */
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

/**
 * Encourage castling lol
 * I just wanted to see castling more often so I tacked some weight onto it.
 */
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

/**
 * Creates an aggregate score using all evaluation methods
 */
double Eval::simple_evaluation(const Node *n) {
  if (detect_checkmate(n) == 0) {
    double sum = material_evaluation(n) + mobility_evaluation(n) +
                 check_bonus(n) + castle_bonus(n);
    return sum;
  }
  return 1'000 * detect_checkmate(n);
}

/**
 * wrapper for simple_evaluation()
 */
double Eval::eval(const Node *n) { return simple_evaluation(n); }

// UNUSED
// ----------------------------------------------------------------------------------------

/**
 * @brief Calculates the material ratio for a given chess board.
 * @details intended to encourage trading when up on material, and discourage
 * trading when down on material
 * @param board Pointer to the Board object representing the chess board.
 * @return The ratio of material advantage to total material. Pos for white neg
 * for black. indicates an advantage for black.
 */
double Eval::material_ratio(const Board *board) {
  double wscore = 0;
  double bscore = 0;
  for (Square sq = s::a8; sq >= s::h1; --sq) {
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

/**
 * In the first ten moves of the game, penalize queen movements, encourage other
 * piece movements
 * @note is not needed when an opening book is in use
 */
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
