#include "../include/Game_State.h"

/**
 * @brief Returns the castling ability of the game state in Forsyth-Edwards Notation (FEN).
 * @return A string representing the castling ability in FEN notation.
 */
std::string Game_State::fen_castling_ability() const
{
    std::string s;
    if (!castle_K && !castle_Q && !castle_k && !castle_q) { s += '-'; }
    else {
        if (castle_K) { s += 'K'; }
        if (castle_Q) { s += 'Q'; }
        if (castle_k) { s += 'k'; }
        if (castle_q) { s += 'q'; }
    }
    return s;
}

/**
 * @brief Returns a string in FEN notation for the en passant targets.
 * @return A string in FEN notation for the en passant targets.
 *         If there are no en passant targets, "-" is returned.
 */
std::string Game_State::fen_en_passant_targets() const
{
    return en_passant_target.empty() ? "-" : en_passant_target;
}

/**
 * @brief Returns the value of the half move clock.
 * @details The half move clock represents the number of half moves since the last capture or pawn advance, and is used
 * for the fifty move rule.
 * @return The value of the half move clock as a string.
 */
std::string Game_State::fen_half_move_clock() const
{
    return std::to_string(half_move_clock);
}

/**
 * @brief Returns the full move number as a string.
 * @details The full move number represents the number of full moves made in the game.
 * It starts at 1 and increments after black's move.
 * @return The full move number as a string.
 */
std::string Game_State::fen_full_move_number() const
{
    return std::to_string(full_move_number);
}

/**
 * @brief Clears the current game state.
 * @details set everything to 0/false/empty
 */
void Game_State::clear()
{
    half_move_clock = full_move_number = 0;
    castle_K = castle_Q = castle_k = castle_q = false;
    en_passant_target.clear();
}

/**
 * @brief Sets the castling ability based on the given string.
 * If the input string is "-", all castling abilities are set to false. Otherwise,
 * each character in the string represents a castling ability. The characters 'K', 'Q', 'k', 'q'
 * correspond to castle_K, castle_Q, castle_k, castle_q respectively. If a character is present
 * in the string, the corresponding castling ability is set to true.
 * @param s The input string representing the castling ability.
 */
void Game_State::set_castling_ability(const std::string& s)
{
    if (s == "-") {
        castle_K = castle_Q = false;
        castle_k = castle_q = false;
    }
    else {
        std::unordered_map<char, bool&> castling_map = {
                {'K', castle_K},
                {'Q', castle_Q},
                {'k', castle_k},
                {'q', castle_q}
        };
        for (const auto& ch : s) {
            for (auto& [key, value] : castling_map) {
                if (ch == key) {
                    value = true;
                }
            }
        }
    }
}

Game_State& Game_State::operator=(const Game_State& rhs)
{
    if (this != &rhs) {
        active_color = rhs.active_color;
        castle_K = rhs.castle_K;
        castle_Q = rhs.castle_Q;
        castle_k = rhs.castle_k;
        castle_q = rhs.castle_q;
        en_passant_target = rhs.en_passant_target;
        en_passant_set = rhs.en_passant_set;
        half_move_clock = rhs.half_move_clock;
        full_move_number = rhs.full_move_number;
        black_inCheck = rhs.black_inCheck;
        white_inCheck = rhs.white_inCheck;
    }
    return *this;
}
