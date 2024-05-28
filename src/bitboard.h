#ifndef SRC_BITBOARD_H_
#define SRC_BITBOARD_H_

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

//----------------------------------------------------------------------------------------------------------------------
// BEGIN Square

enum class Square {
    h1, g1, f1, e1, d1, c1, b1, a1,
    h2, g2, f2, e2, d2, c2, b2, a2,
    h3, g3, f3, e3, d3, c3, b3, a3,
    h4, g4, f4, e4, d4, c4, b4, a4,
    h5, g5, f5, e5, d5, c5, b5, a5,
    h6, g6, f6, e6, d6, c6, b6, a6,
    h7, g7, f7, e7, d7, c7, b7, a7,
    h8, g8, f8, e8, d8, c8, b8, a8
};

Square& operator--(Square& square)
{
    square = static_cast<Square>(static_cast<int>(square) - 1);
    return square;
}

// TODO overload stream extraction operator

// END Square
//----------------------------------------------------------------------------------------------------------------------
// BEGIN Game_State

/**
 * @brief The Game_State struct represents the current state of a game.
 * @details  This struct stores information such as the active color, castling ability, en passant targets,
 * half move clock, and full move number.
 */
struct Game_State {
    [[nodiscard]] char fen_active_color() const { return active_color_clock % 2 == 0 ? 'w' : 'b'; }

    [[nodiscard]] std::string fen_castling_ability() const;
    [[nodiscard]] std::string fen_en_passant_targets() const;
    [[nodiscard]] std::string fen_half_move_clock() const;
    [[nodiscard]] std::string fen_full_move_number() const;

    ushort active_color_clock{};    // increments after every move. even for white, odd for black.
    bool castle_K = true;           // white can castle king side
    bool castle_Q = true;           // white can castle queen side
    bool castle_k = true;           // black can castle king side
    bool castle_q = true;           // black can castle queen side
    std::vector<Square> en_passant_targets{};   // vector of en passant target squares
    uint8_t half_move_clock{};      // for the 50 move rule
    uint16_t full_move_number = 1;  // starts at 1, increments after black moves
};

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
    // IMPORTANT en passant possibility is recorded WHENEVER a pawn moves two squares
    // and is only valid for the opponent's next move.
    std::string s;
    if (en_passant_targets.empty()) {
        s = '-';
    }
    else {
        // write the code for the thing lol
    }
    return s;
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

// END Game_State
//----------------------------------------------------------------------------------------------------------------------
// BEGIN Board

// bits are in standard starting position
struct Board {
    [[nodiscard]] std::string fen_piece_placement() const;

    uint64_t b_pawn = 0b00000000'11111111'00000000'00000000'00000000'00000000'00000000'00000000;
    uint64_t b_night = 0b01000010'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
    uint64_t b_bishop = 0b00100100'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
    uint64_t b_rook = 0b10000001'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
    uint64_t b_queen = 0b00010000'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
    uint64_t b_king = 0b00001000'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
    uint64_t w_Pawn = 0b00000000'00000000'00000000'00000000'00000000'00000000'11111111'00000000;
    uint64_t w_Night = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'01000010;
    uint64_t w_Bishop = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00100100;
    uint64_t w_Rook = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'10000001;
    uint64_t w_Queen = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00010000;
    uint64_t w_King = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00001000;

    Game_State game_state;
};

bool isBitSet(uint64_t n, Square sq)
{
    return n & (1ULL << static_cast<int>(sq));
}

std::string Board::fen_piece_placement() const
{
    std::string piece_placement;
    // Mapping of pieces to their FEN representation
    std::array<std::pair<char, uint64_t>, 12> piece_map = {
            std::make_pair('p', b_pawn),
            std::make_pair('n', b_night),
            std::make_pair('b', b_bishop),
            std::make_pair('r', b_rook),
            std::make_pair('q', b_queen),
            std::make_pair('k', b_king),
            std::make_pair('P', w_Pawn),
            std::make_pair('N', w_Night),
            std::make_pair('B', w_Bishop),
            std::make_pair('R', w_Rook),
            std::make_pair('Q', w_Queen),
            std::make_pair('K', w_King)
    };
    int emptySquares = 0;   // to count empty squares
    for (Square sq = Square::a8; sq >= Square::h1; --sq) {
        bool pieceFound = false;
        // search the square for each type of piece
        for (const auto& piece : piece_map) {
            if (isBitSet(piece.second, sq)) {
                if (emptySquares > 0) {
                    piece_placement += std::to_string(emptySquares);
                    emptySquares = 0;
                }
                piece_placement += piece.first;
                pieceFound = true;
                break;
            }
        }
        // count empty squares
        if (!pieceFound) {
            emptySquares++;
        }
        // slash for each new row (excluding an eighth slash)
        if (sq != Square::h1 && static_cast<int>(sq) % 8 == 0) {
            // include empty square count
            if (emptySquares > 0) {
                piece_placement += std::to_string(emptySquares);
                emptySquares = 0;
            }
            piece_placement += '/';
        }
    }
    return piece_placement;
}

// END Board
//----------------------------------------------------------------------------------------------------------------------

/**
 * @brief Export the current state of the board as FEN (Forsyth–Edwards Notation) string.
 * @details
 * 1. piece placement \n
 * 2. active color \n
 * 3. castling ability '-' if neither side has the ability \n
 * 4. en passant target squares '-' if there are no target squares \n
 * 5. half move clock, for fifty move rule
 * 6. full move number
 * @param board Pointer to the board object.
 * @return std::string Returns the FEN string representing the board state.
 */
std::string export_fen(const Board *board)
{
    std::string fen;    // string to be exported
    fen += board->fen_piece_placement();
    fen += ' ';
    fen += board->game_state.fen_active_color();
    fen += ' ';
    fen += board->game_state.fen_castling_ability();
    fen += ' ';
    fen += board->game_state.fen_en_passant_targets();
    fen += ' ';
    fen += board->game_state.fen_half_move_clock();
    fen += ' ';
    fen += board->game_state.fen_full_move_number();
    return fen;
}

#endif  // SRC_BITBOARD_H_
