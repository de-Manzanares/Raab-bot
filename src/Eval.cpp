#include "../include/Eval.h"

struct Node;

double Eval::CHECK_BONUS = 0.25;                    // gives weight to checks 0.25
double Eval::MOBILITY_MULTIPLIER = 1.0 / 100.0;     // gives weight for each legal move 1.0
double Eval::CASTLE_BONUS = 0.5;                    // gives weight to castling 0.5
double Eval::RATIO_MULTIPLIER = 2.0;                // gives weight to ratio of material advantage

/// material value
std::unordered_map<char, int> Eval::material_value = {
        {'Q', 900}, {'R', 500}, {'B', 310}, {'N', 300}, {'P', 100},
        {'q', -900}, {'r', -500}, {'b', -310}, {'n', -300}, {'p', -100}};

/**
 * @brief Calculates the material ratio for a given chess board.
 * @details intended to encourage trading when up on material, and discourage trading when down on material
 * @param board Pointer to the Board object representing the chess board.
 * @return The ratio of material advantage to total material. Pos for white neg for black.
 * indicates an advantage for black.
 */
double Eval::material_ratio(const Board *board)
{
    double wscore = 0;
    double bscore = 0;
    double ratio = 0;
    for (Square sq = Square::a8; sq >= Square::h1; --sq) {
        if (!board->is_empty(sq)) {
            if (board->is_white(sq)) { wscore += material_value[board->what_piece(sq)]; }
            else if (board->is_black(sq)) { bscore += material_value[board->what_piece(sq)]; }
        }
    }
    if (wscore == -bscore) { return 0; }
    return RATIO_MULTIPLIER * ((wscore + bscore) / (wscore - bscore));
}

/**
 * @brief Evaluates the material value on the given chess board.
 * @details This function calculates the material value of the chess pieces on the given chess board.
 * @param board A pointer to the chess board to be evaluated.
 * @return The material value of the chess pieces on the board divided by 100.
 */
double Eval::material_evaluation(const Board *board)
{
    double sum = 0;
    for (Square sq = Square::a8; sq >= Square::h1; --sq) {
        if (!board->is_empty(sq)) {
            sum += material_value[board->what_piece(sq)];
        }
    }
    return sum / 100;
}

/**
 * @brief Detects if the given chess board is in a checkmate position.
 * @detlails Check whether there are any legal moves available for the active player. If there are no
 * legal moves available, this is checkmate.
 * @param board The chess board for which to detect checkmate.
 * @return -1 if White is in checkmate, 1 if Black is in checkmate, 0 if neither color is in checkmate.
 */
int Eval::detect_checkmate(const Board *board)
{
    ulong number_of_moves = 0;

    // detect white checkmate
    if (board->game_state.active_color == Color::white) {
        if (board->game_state.in_check_white) {
            for (const auto& [sq, moves] : board->maps->move_map_white) { number_of_moves += moves.size(); }
            if (number_of_moves == 0) { return -1; }
        }
    }
    // detect black checkmate
    if (board->game_state.active_color == Color::black) {
        if (board->game_state.in_check_black) {
            for (const auto& [sq, moves] : board->maps->move_map_black) { number_of_moves += moves.size(); }
            if (number_of_moves == 0) { return 1; }
        }
    }
    // if neither is in checkmate
    return 0;
}

/**
 * @brief Evaluates the mobility of a given chess board.
 * @details Adds score for each legal move available to a side.
 * @param board A pointer to the chess board.
 * @return The mobility score.
 */
double Eval::mobility_evaluation(const Board *board)
{
    double score = 0;
    for (const auto& [sq, moves] : board->maps->move_map_white) {
        score += static_cast<double>(moves.size());
    }
    for (const auto& [sq, moves] : board->maps->move_map_black) {
        score -= static_cast<double>(moves.size());
    }
    return score * MOBILITY_MULTIPLIER;
}

/**
 * Calculates the bonus score for a given board based on whether a player is in check.
 * @param board The board to check for in-check status.
 * @return (+) score is black is in check, (-) score if white is in check
 */
double Eval::check_bonus(const Board *board)
{
    double score = 0;
    if (board->game_state.in_check_white) { score -= CHECK_BONUS; }
    if (board->game_state.in_check_black) { score += CHECK_BONUS; }
    return score;
}

/**
 * Creates an aggregate score using all evaluation methods
 */
double Eval::simple_evaluation(const Board *board)
{
    if (detect_checkmate(board) == 0) {
        double sum = material_evaluation(board)
                + mobility_evaluation(board)
                + check_bonus(board);
        return sum;
    }
    return 1'000 * detect_checkmate(board);
}

/**
 * wrapper for simple_evaluation()
 */
double Eval::eval(const Board *board)
{
    return simple_evaluation(board);
}

/**
 * In the first ten moves of the game, penalize queen movements, encourage other piece movements
 * @note is not needed when an opening book is in use
 */
double Eval::discourage_early_queen_movement(const Node *node)
{
    double score = 0;
    if (node->_board.game_state.full_move_number <= 10) {
        if (node->parent != nullptr) {
            if (node->parent->_board.is_queen(node->_from)) {
                if (node->parent->_board.game_state.active_color == Color::white) { score -= .9; }
                if (node->parent->_board.game_state.active_color == Color::black) { score += .9; }
            }
            else if (!node->parent->_board.is_pawn(node->_from)) {
                if (node->parent->_board.game_state.active_color == Color::white) { score += .5; }
                if (node->parent->_board.game_state.active_color == Color::black) { score -= .5; }
            }
        }
    }
    return score;
}

/**
 * Encourage castling lol
 * I just wanted to see castling more often so I tacked some weight onto it.
 */
double Eval::castle_bonus(Node *n)
{
    double score = 0;
    if (n->parent != nullptr) {
        if (n->parent->_board.is_white_king(n->_from) && n->_from == Square::e1) {
            if (n->_to == Square::g1 || n->_to == Square::c1) { score += CASTLE_BONUS; }
        }
        else if (n->parent->_board.is_black_king(n->_from) && n->_from == Square::e8) {
            if (n->_to == Square::g8 || n->_to == Square::c8) { score -= CASTLE_BONUS; }
        }
    }
    return score;
}