#ifndef INCLUDE_GAME_STATE_H_
#define INCLUDE_GAME_STATE_H_

#include <string>
#include <unordered_map>
#include "Square.h"

/**
 * @brief Represents the current state of a game.
 * @details  Stores information such as the active color, castling ability, en passant targets,
 * half move clock, and full move number.
 */
struct Game_State {
    Game_State& operator=(const Game_State& rhs);

    // give game state data in fen format
    [[nodiscard]] char fen_active_color() const { return active_color == Color::white ? 'w' : 'b'; }

    [[nodiscard]] std::string fen_castling_ability() const;
    [[nodiscard]] std::string fen_en_passant_targets() const;
    [[nodiscard]] std::string fen_half_move_clock() const;
    [[nodiscard]] std::string fen_full_move_number() const;

    void clear();
    void set_castling_ability(const std::string& s);

    // game-state data
    Color active_color = Color::white;  // flips after every move. starts with white.
    bool castle_K = true;               // white can castle king side
    bool castle_Q = true;               // white can castle queen side
    bool castle_k = true;               // black can castle king side
    bool castle_q = true;               // black can castle queen side
    std::string en_passant_target;      // en passant target
    bool en_passant_set = false;        // to note if en passant was just set during the turn
    uint half_move_clock{};             // for the 50 move rule
    uint full_move_number = 1;          // starts at 1, increments after black moves

    bool in_check_black = false;        // black king is in check
    bool in_check_white = false;        // white king is in check
};

#endif  // INCLUDE_GAME_STATE_H_
