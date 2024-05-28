#ifndef SRC_BITBOARD_H_
#define SRC_BITBOARD_H_

#include <cstdint>
#include <string>
#include <sstream>
#include <utility>
#include <vector>

//----------------------------------------------------------------------------------------------------------------------
// BEGIN Square

// do not make underlying uint8_t
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

    void clear();

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
    // FEN records the square just behind any pawn that has made a two-square push forward in the latest move.
    // As such, whenever a pawn makes a two-square move, the en passant square is recorded.
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

/**
 * @brief Clears the current game state.
 * @details set everything to 0/false/empty
 */
void Game_State::clear()
{
    active_color_clock = half_move_clock = full_move_number = 0;
    castle_K = castle_Q = castle_k = castle_q = false;
    en_passant_targets.clear();
}

// END Game_State
//----------------------------------------------------------------------------------------------------------------------
// BEGIN Board

// bits are in standard starting position
struct Board {
    [[nodiscard]] std::string fen_piece_placement() const;
    void clear();
    void place_piece(Square square, const char& ch);

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

    std::unordered_map<char, uint64_t&> piece_map = {
            {'p', b_pawn},
            {'n', b_night},
            {'b', b_bishop},
            {'r', b_rook},
            {'q', b_queen},
            {'k', b_king},
            {'P', w_Pawn},
            {'N', w_Night},
            {'B', w_Bishop},
            {'R', w_Rook},
            {'Q', w_Queen},
            {'K', w_King}};

    // active color, half-move, etc ..
    Game_State game_state;
};

bool isBitSet(uint64_t n, Square sq)
{
    return n & (1ULL << static_cast<int>(sq));
}

/**
 * @brief Returns the FEN representation of the piece placement on the board.
 * @return The FEN representation of the piece placement on the board.
 */
std::string Board::fen_piece_placement() const
{
    std::string piece_placement;
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
            // to flush last empty squares
            if (sq == Square::h1) {
                piece_placement += std::to_string(emptySquares);
                emptySquares = 0;
            }
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

/**
 * @brief Remove all pieces, and clear the game state.
 */
void Board::clear()
{
    // remove all the pieces
    b_pawn = b_night = b_bishop = b_rook = b_queen = b_king = 0;
    w_Pawn = w_Night = w_Bishop = w_Rook = w_Queen = w_King = 0;
    // clear the game state
    game_state.clear();
}

void Board::place_piece(Square square, const char& ch)
{
    // assumes the given square is empty
    for (auto& piece : piece_map) {
        if (ch == piece.first) {
            piece.second |= 1ULL << static_cast<uint>(square);
        }
    }
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

// TODO check if fen is valid before/during import

bool is_piece(const char& ch)
{
    return ch == 'p' || ch == 'n' || ch == 'b' || ch == 'r' || ch == 'q' || ch == 'k' ||
            ch == 'P' || ch == 'N' || ch == 'B' || ch == 'R' || ch == 'Q' || ch == 'K';
}

void import_fen(Board *board, const std::string& fen)
{
    uint8_t gs_counter{};
    std::string gs;
    std::string temp;
    Square square = Square::a8;

    board->clear();

    // set pieces
    for (const auto& ch : fen) {
        if (is_piece(ch)) {
            board->place_piece(square, ch);
            --square;
        }
        if (std::isdigit(ch)) {
            for (auto i = 0; i < ch - '0'; i++) {
                --square;
            }
        }
        if (ch == ' ') {
            gs_counter++;
            break;
        }
        gs_counter++;
    }
    gs = fen.substr(gs_counter);
    std::istringstream iss(gs);

    // set games state variables
    char active_color;
    std::string castling_ability;
    std::string en_passant_targets;
    uint16_t half_move_clock;
    uint16_t full_move_number;
    // gather data
    iss >> active_color >> castling_ability >> en_passant_targets >> half_move_clock >> full_move_number;

    // assign to board variables
    active_color == 'w' ?
            board->game_state.active_color_clock = 0 : board->game_state.active_color_clock = 1;
    if (castling_ability == "-") {
        board->game_state.castle_K = board->game_state.castle_Q = false;
        board->game_state.castle_k = board->game_state.castle_q = false;
    }
    else {
        std::unordered_map<char, bool&> castling_map = {
                {'K', board->game_state.castle_K},
                {'Q', board->game_state.castle_Q},
                {'k', board->game_state.castle_k},
                {'q', board->game_state.castle_q}
        };
        for (const auto& ch : castling_ability) {
            for (auto& [key, value] : castling_map) {
                if (ch == key) {
                    value = true;
                }
            }
        }
    }
    // TODO en passant squares
    board->game_state.half_move_clock = half_move_clock;
    board->game_state.full_move_number = full_move_number;
}

#endif  // SRC_BITBOARD_H_
