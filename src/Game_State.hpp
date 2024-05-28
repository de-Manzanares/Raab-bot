#ifndef CHESSENGINE_GAME_STATE_HPP
#define CHESSENGINE_GAME_STATE_HPP

#include <string>
#include <unordered_map>

//----------------------------------------------------------------------------------------------------------------------
// BEGIN Game_State

/**
 * @brief Represents the current state of a game.
 * @details  Stores information such as the active color, castling ability, en passant targets,
 * half move clock, and full move number.
 */
struct Game_State {
    [[nodiscard]] char fen_active_color() const { return active_color_clock % 2 == 0 ? 'w' : 'b'; }

    [[nodiscard]] std::string fen_castling_ability() const;
    [[nodiscard]] std::string fen_en_passant_targets() const;
    [[nodiscard]] std::string fen_half_move_clock() const;
    [[nodiscard]] std::string fen_full_move_number() const;
    void clear();
    void set_castling_ability(const std::string& s);

    uint active_color_clock{};  // increments after every move. even for white, odd for black.
    bool castle_K = true;       // white can castle king side
    bool castle_Q = true;       // white can castle queen side
    bool castle_k = true;       // black can castle king side
    bool castle_q = true;       // black can castle queen side
    std::string en_passant_target;   // en passant target
    uint half_move_clock{};     // for the 50 move rule
    uint full_move_number = 1;  // starts at 1, increments after black moves
};

// END Game_State
//----------------------------------------------------------------------------------------------------------------------

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
    // FEN records the square just behind any pawn that has made a two-square push forward in the latest move.
    // As such, whenever a pawn makes a two-square move, the en passant square is recorded.
    // IMPORTANT en passant possibility is recorded WHENEVER a pawn moves two squares
    // and is only valid for the opponent's next move.

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
    // half move clock: number of half moves since the last capture or pawn advance, used for the fifty move rule
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
    // full move number: number of full moves. Starts at 1 and increments after blacks move.
    return std::to_string(full_move_number);
}

/**
 * @brief Clears the current game state.
 * @details set everything to 0/false/empty
 */
void Game_State::clear()
{
    active_color_clock = half_move_clock = full_move_number = 0;
    castle_K = castle_Q = castle_k = castle_q = false;
    en_passant_target.clear();
}

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


#endif //CHESSENGINE_GAME_STATE_HPP
