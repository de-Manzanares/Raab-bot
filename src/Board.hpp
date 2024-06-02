#ifndef SRC_BITBOARD_H_
#define SRC_BITBOARD_H_

#include <cstdint>
#include <string>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Square.hpp"
#include "boundary-detection.hpp"
#include "Game_State.hpp"

// TODO make helper methods private for organization

//----------------------------------------------------------------------------------------------------------------------
// BEGIN Board

// bits are in standard starting position
struct Board {
    [[nodiscard]] std::string fen_piece_placement() const;
    void clear();
    void place_piece(Square square, const char& ch);
    uint set_pieces(const std::string& fen);
    // should it return something?
    char what_piece(uint sq) const;
    std::vector<Square> influence(Square sq) const;

    // I think there may need to be three maps?
    // an "influence" map - influence of the piece
    // a "move" map - where the piece can actually move
    // an xray map - where the piece could move if there were no pieces in the way

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

    // can we have three wide?

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

    std::unordered_map<Square, std::vector<Square>> move_map_white{};
    std::unordered_map<Square, std::vector<Square>> move_map_black{};
    std::unordered_map<Square, std::vector<Square>> influence_map_white{};
    std::unordered_map<Square, std::vector<Square>> influence_map_black{};
    void update_influence_maps();

    char what_piece(Square sq) const;
    std::vector<Square> influence_rook(Square sq) const;
    bool is_white_rook(Square sq) const;
    bool is_black_rook(Square sq) const;
    bool is_rook(Square sq) const;
    bool is_white_bishop(Square sq) const;
    bool is_black_bishop(Square sq) const;
    bool is_bishop(Square sq) const;
    std::vector<Square> influence_bishop(Square sq) const;
    bool is_white_queen(Square sq) const;
    bool is_black_queen(Square sq) const;
    bool is_queen(Square sq) const;
    std::vector<Square> influence_queen(Square sq) const;
    bool is_white_knight(Square sq) const;
    bool is_black_knight(Square sq) const;
    bool is_knight(Square sq) const;
    std::vector<Square> influence_knight(Square sq) const;
    bool is_white_king(Square sq) const;
    bool is_black_king(Square sq) const;
    bool is_king(Square sq) const;
    std::vector<Square> influence_king(Square sq) const;
    bool is_white_pawn(Square sq) const;
    bool is_black_pawn(Square sq) const;
    bool is_pawn(Square sq) const;
    std::vector<Square> influence_pawn(Square sq) const;
    bool is_black(Square sq) const;
    bool is_white(Square sq) const;
    Color what_color(Square sq) const;
    bool is_same_color(Square sq, Color color) const;
    bool is_empty(Square sq) const;
    bool is_opposite_rook(Square sq, Color c) const;
    bool is_opposite_bishop(Square sq, Color c) const;
    bool is_opposite_queen(Square sq, Color c) const;
    bool is_opposite_knight(Square sq, Color c) const;
    bool is_opposite_king(Square sq, Color c) const;
    bool is_opposite_pawn(Square sq, Color c) const;
    bool is_opposite_color(Square sq, Color c) const;
    std::vector<Square> legal_moves(Square sq) const;
    std::vector<Square> legal_moves_pawn(Square sq) const;
    void update_white_king_moves_old(Square square_K);
    void update_move_maps();
    void remove_same_color_squares(std::unordered_map<Square, std::vector<Square>> *map, Color color);
    Square assign_from_influence_map_exclude_pawns_and_kings(std::unordered_map<Square, std::vector<Square>> *to,
            std::unordered_map<Square, std::vector<Square>> *from);
    void update_white_king_moves(Square square_K);
    void update_black_king_moves(Square square_K);
    void print_move_map(Color color) const;
};

// END Board
//----------------------------------------------------------------------------------------------------------------------

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
 * @brief Remove all pieces and clear the game state.
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

bool is_piece(const char& ch)
{
    return ch == 'p' || ch == 'n' || ch == 'b' || ch == 'r' || ch == 'q' || ch == 'k' ||
            ch == 'P' || ch == 'N' || ch == 'B' || ch == 'R' || ch == 'Q' || ch == 'K';
}

uint Board::set_pieces(const std::string& fen)
{
    Square square = Square::a8;
    uint counter{};
    for (const auto& ch : fen) {
        if (is_piece(ch)) {
            this->place_piece(square, ch);
            --square;
        }
        if (std::isdigit(ch)) {
            for (auto i = 0; i < ch - '0'; i++) {
                --square;
            }
        }
        if (ch == ' ') {
            counter++;
            break;
        }
        counter++;
    }
    return counter;
}

//----------------------------------------------------------------------------------------------------------------------
// BEGIN piece detection

char Board::what_piece(Square sq) const
{
    for (const auto& [key, value] : piece_map) {
        if (value & (1ULL << static_cast<int>(sq))) { return key; }
    }
    return ' ';
}

char Board::what_piece(uint sq) const
{
    for (const auto& [key, value] : piece_map) {
        if (value & (1ULL << sq)) { return key; }
    }
    return ' ';
}

bool Board::is_white_rook(Square sq) const
{
    return what_piece(sq) == 'R';
}

bool Board::is_black_rook(Square sq) const
{
    return what_piece(sq) == 'r';
}

bool Board::is_rook(Square sq) const
{
    return is_white_rook(sq) || is_black_rook(sq);
}

bool Board::is_white_bishop(Square sq) const
{
    return what_piece(sq) == 'B';
}

bool Board::is_black_bishop(Square sq) const
{
    return what_piece(sq) == 'b';
}

bool Board::is_bishop(Square sq) const
{
    return is_white_bishop(sq) || is_black_bishop(sq);
}

bool Board::is_white_queen(Square sq) const
{
    return what_piece(sq) == 'Q';
}

bool Board::is_black_queen(Square sq) const
{
    return what_piece(sq) == 'q';
}

bool Board::is_queen(Square sq) const
{
    return is_white_queen(sq) || is_black_queen(sq);
}

bool Board::is_white_knight(Square sq) const
{
    return what_piece(sq) == 'N';
}

bool Board::is_black_knight(Square sq) const
{
    return what_piece(sq) == 'n';
}

bool Board::is_knight(Square sq) const
{
    return is_white_knight(sq) || is_black_knight(sq);
}

bool Board::is_white_king(Square sq) const
{
    return what_piece(sq) == 'K';
}

bool Board::is_black_king(Square sq) const
{
    return what_piece(sq) == 'k';
}

bool Board::is_king(Square sq) const
{
    return is_white_king(sq) || is_black_king(sq);
}

bool Board::is_white_pawn(Square sq) const
{
    return what_piece(sq) == 'P';
}

bool Board::is_black_pawn(Square sq) const
{
    return what_piece(sq) == 'p';
}

bool Board::is_pawn(Square sq) const
{
    return is_white_pawn(sq) || is_black_pawn(sq);
}

bool Board::is_white(Square sq) const
{
    return is_white_pawn(sq) || is_white_rook(sq) || is_white_bishop(sq) || is_white_knight(sq) ||
            is_white_queen(sq) || is_white_king(sq);
}

bool Board::is_black(Square sq) const
{
    return is_black_pawn(sq) || is_black_rook(sq) || is_black_bishop(sq) || is_black_knight(sq) ||
            is_black_queen(sq) || is_black_king(sq);
}

Color Board::what_color(Square sq) const
{
    Color color;
    if (is_white(sq)) { color = Color::white; }
    else if (is_black(sq)) { color = Color::black; }
    else { color = Color::none; }
    return color;
}

bool Board::is_same_color(Square sq, Color color) const
{
    return what_color(sq) == color;
}

bool Board::is_empty(Square sq) const
{
    return what_piece(sq) == ' ';
}

bool Board::is_opposite_rook(Square sq, Color c) const
{
    return c == Color::white ? is_black_rook(sq) : is_white_rook(sq);
}

bool Board::is_opposite_bishop(Square sq, Color c) const
{
    return c == Color::white ? is_black_bishop(sq) : is_white_bishop(sq);
}

bool Board::is_opposite_queen(Square sq, Color c) const
{
    return c == Color::white ? is_black_queen(sq) : is_white_queen(sq);
}

bool Board::is_opposite_knight(Square sq, Color c) const
{
    return c == Color::white ? is_black_knight(sq) : is_white_knight(sq);
}

bool Board::is_opposite_king(Square sq, Color c) const
{
    return c == Color::white ? is_black_king(sq) : is_white_king(sq);
}

bool Board::is_opposite_pawn(Square sq, Color c) const
{
    return c == Color::white ? is_black_pawn(sq) : is_white_pawn(sq);
}

bool Board::is_opposite_color(Square sq, Color c) const
{
    return c == Color::white ? is_black(sq) : is_white(sq);
}

// END piece detection
//----------------------------------------------------------------------------------------------------------------------
// BEGIN influence map rook

std::vector<Square> Board::influence_rook(Square sq) const
{
    std::vector<Square> influence{};
    // vertical up
    for (auto square = sq + 8; !is_upper_vertical_boundary(square - 8); square = square + 8) {
        influence.push_back(square);
        if (what_piece(square) != ' ') { break; }
    }
    // vertical down
    for (auto square = sq - 8; !is_lower_vertical_boundary(square + 8); square = square - 8) {
        influence.push_back(square);
        if (what_piece(square) != ' ') { break; }
    }
    // horizontal right
    for (auto square = sq - 1; !is_right_horizontal_boundary(square + 1); square = square - 1) {
        influence.push_back(square);
        if (what_piece(square) != ' ') { break; }
    }
    // horizontal left
    for (auto square = sq + 1; !is_left_horizontal_boundary(square - 1); square = square + 1) {
        influence.push_back(square);
        if (what_piece(square) != ' ') { break; }
    }
    return influence;
}

// END influence map rook
//----------------------------------------------------------------------------------------------------------------------
// BEGIN influence map bishop

std::vector<Square> Board::influence_bishop(Square sq) const
{
    std::vector<Square> influence{};
    // up left
    for (auto square = sq + 9;
         !is_upper_vertical_boundary(square - 9) && !is_left_horizontal_boundary(square - 9);
         square = square + 9) {
        influence.push_back(square);
        if (what_piece(square) != ' ') { break; }
    }
    // up right
    for (auto square = sq + 7;
         !is_upper_vertical_boundary(square - 7) && !is_right_horizontal_boundary(square - 7);
         square = square + 7) {
        influence.push_back(square);
        if (what_piece(square) != ' ') { break; }
    }
    // down right
    for (auto square = sq - 9;
         !is_lower_vertical_boundary(square + 9) && !is_right_horizontal_boundary(square + 9);
         square = square - 9) {
        influence.push_back(square);
        if (what_piece(square) != ' ') { break; }
    }
    // down left
    for (auto square = sq - 7;
         !is_lower_vertical_boundary(square + 7) && !is_left_horizontal_boundary(square + 7);
         square = square - 7) {
        influence.push_back(square);
        if (what_piece(square) != ' ') { break; }
    }
    return influence;
}

// END influence map bishop
//----------------------------------------------------------------------------------------------------------------------
// BEGIN influence map queen

std::vector<Square> Board::influence_queen(Square sq) const
{
    std::vector<Square> v1 = influence_rook(sq);
    std::vector<Square> v2 = influence_bishop(sq);

    v1.insert(v1.end(), v2.begin(), v2.end());

    return v1;
}

// END influence map queen
//----------------------------------------------------------------------------------------------------------------------
// BEGIN influence map knight

std::vector<Square> Board::influence_knight(Square sq) const
{
    using s = Square;
    std::vector<Square> influence{};
    int isq = static_cast<int>(sq);

    // center 4x4 - 16 squares
    if (isq >= 18 && isq <= 21 || isq >= 26 && isq <= 29 || isq >= 34 && isq <= 37 || isq >= 42 && isq <= 45) {
        influence.push_back(sq - (2 * 8 + 1));     // down right
        influence.push_back(sq - (2 * 8 - 1));     // down left
        influence.push_back(sq - (8 + 2));         // right down
        influence.push_back(sq - (8 - 2));         // left down
        influence.push_back(sq + (8 - 2));         // right up
        influence.push_back(sq + (8 + 2));         // left up
        influence.push_back(sq + (2 * 8 - 1));     // up right
        influence.push_back(sq + (2 * 8 + 1));     // up left
    }
        // b file - 4 squares
    else if (sq == Square::b3 || sq == Square::b4 || sq == Square::b5 || sq == Square::b6) {
        influence.push_back(sq - (2 * 8 + 1));     // down right
        influence.push_back(sq - (2 * 8 - 1));     // down left
        influence.push_back(sq - (8 + 2));         // right down
        influence.push_back(sq + (8 - 2));         // right up
        influence.push_back(sq + (2 * 8 - 1));     // up right
        influence.push_back(sq + (2 * 8 + 1));     // up left
    }
        // g files - 4 squares
    else if (sq == Square::g3 || sq == Square::g4 || sq == Square::g5 || sq == Square::g6) {
        influence.push_back(sq - (2 * 8 + 1));     // down right
        influence.push_back(sq - (2 * 8 - 1));     // down left
        influence.push_back(sq - (8 - 2));         // left down
        influence.push_back(sq + (8 + 2));         // left up
        influence.push_back(sq + (2 * 8 - 1));     // up right
        influence.push_back(sq + (2 * 8 + 1));     // up left
    }
        // 2nd rank - 4 squares
    else if (isq >= static_cast<int>(Square::f2) && isq <= static_cast<int>(Square::c2)) {
        influence.push_back(sq - (8 + 2));         // right down
        influence.push_back(sq - (8 - 2));         // left down
        influence.push_back(sq + (8 - 2));         // right up
        influence.push_back(sq + (8 + 2));         // left up
        influence.push_back(sq + (2 * 8 - 1));     // up right
        influence.push_back(sq + (2 * 8 + 1));     // up left
    }
        // 7th rank - 4 squares
    else if (isq >= static_cast<int>(Square::f7) && isq <= static_cast<int>(Square::c7)) {
        influence.push_back(sq - (2 * 8 + 1));     // down right
        influence.push_back(sq - (2 * 8 - 1));     // down left
        influence.push_back(sq - (8 + 2));         // right down
        influence.push_back(sq - (8 - 2));         // left down
        influence.push_back(sq + (8 - 2));         // right up
        influence.push_back(sq + (8 + 2));         // left up
    }
        // a file - 4 squares
    else if (sq == Square::a3 || sq == Square::a4 || sq == Square::a5 || sq == Square::a6) {
        influence.push_back(sq - (2 * 8 + 1));     // down right
        influence.push_back(sq - (8 + 2));         // right down
        influence.push_back(sq + (8 - 2));         // right up
        influence.push_back(sq + (2 * 8 - 1));     // up right
    }
        // h file - 4 squares
    else if (sq == Square::h3 || sq == Square::h4 || sq == Square::h5 || sq == Square::h6) {
        influence.push_back(sq - (2 * 8 - 1));     // down left
        influence.push_back(sq - (8 - 2));         // left down
        influence.push_back(sq + (8 + 2));         // left up
        influence.push_back(sq + (2 * 8 + 1));     // up left
    }
        // 1st rank - 4 squares
    else if (isq >= static_cast<int>(Square::f1) && isq <= static_cast<int>(Square::c1)) {
        influence.push_back(sq + (8 - 2));         // right up
        influence.push_back(sq + (8 + 2));         // left up
        influence.push_back(sq + (2 * 8 - 1));     // up right
        influence.push_back(sq + (2 * 8 + 1));     // up left
    }
        // 8th rank - 4 squares
    else if (isq >= static_cast<int>(Square::f8) && isq <= static_cast<int>(Square::c8)) {
        influence.push_back(sq - (2 * 8 + 1));     // down right
        influence.push_back(sq - (2 * 8 - 1));     // down left
        influence.push_back(sq - (8 + 2));         // right down
        influence.push_back(sq - (8 - 2));         // left down
    }
        // inside 2's and 7's - 1 square each
    else if (isq == static_cast<int>(Square::g2)) { influence = {s::e1, s::e3, s::h4, s::f4}; }
    else if (isq == static_cast<int>(Square::b2)) { influence = {s::d1, s::d3, s::c4, s::a4}; }
    else if (isq == static_cast<int>(Square::g7)) { influence = {s::h5, s::f5, s::e6, s::e8}; }
    else if (isq == static_cast<int>(Square::b7)) { influence = {s::c5, s::a5, s::d6, s::d8}; }
        // edge 2's and 7's - 1 square each
    else if (isq == static_cast<int>(Square::h2)) { influence = {s::f1, s::f3, s::g4}; }
    else if (isq == static_cast<int>(Square::a2)) { influence = {s::c1, s::c3, s::b4}; }
    else if (isq == static_cast<int>(Square::h7)) { influence = {s::g5, s::f6, s::f8}; }
    else if (isq == static_cast<int>(Square::a7)) { influence = {s::b5, s::c6, s::c8}; }
        // edge 1's and 8's - 1 square each
    else if (isq == static_cast<int>(Square::g1)) { influence = {s::e2, s::h3, s::f3}; }
    else if (isq == static_cast<int>(Square::b1)) { influence = {s::d2, s::c3, s::a3}; }
    else if (isq == static_cast<int>(Square::g8)) { influence = {s::h6, s::f6, s::e7}; }
    else if (isq == static_cast<int>(Square::b8)) { influence = {s::c6, s::a6, s::d7}; }
        // corners - 1 square each
    else if (isq == static_cast<int>(Square::h1)) { influence = {s::f2, s::g3}; }
    else if (isq == static_cast<int>(Square::a1)) { influence = {s::c2, s::b3}; }
    else if (isq == static_cast<int>(Square::h8)) { influence = {s::g6, s::f7}; }
    else if (isq == static_cast<int>(Square::a8)) { influence = {s::b6, s::c7}; }

    return influence;
}

// END influence map knight
//----------------------------------------------------------------------------------------------------------------------
// BEGIN influence map king

std::vector<Square> Board::influence_king(Square sq) const
{
    std::vector<Square> influence{};

    // vertical
    if (!is_upper_vertical_boundary(sq)) { influence.push_back(sq + 8); }
    if (!is_lower_vertical_boundary(sq)) { influence.push_back(sq - 8); }
    // horizontal
    if (!is_right_horizontal_boundary(sq)) { influence.push_back(sq - 1); }
    if (!is_left_horizontal_boundary(sq)) { influence.push_back(sq + 1); }
    // diagonal
    if (!is_lower_vertical_boundary(sq) && !is_right_horizontal_boundary(sq)) { influence.push_back(sq - 9); }
    if (!is_lower_vertical_boundary(sq) && !is_left_horizontal_boundary(sq)) { influence.push_back(sq - 7); }
    if (!is_upper_vertical_boundary(sq) && !is_right_horizontal_boundary(sq)) { influence.push_back(sq + 7); }
    if (!is_upper_vertical_boundary(sq) && !is_left_horizontal_boundary(sq)) { influence.push_back(sq + 9); }

    return influence;
}

// END influence map king
//----------------------------------------------------------------------------------------------------------------------
// BEGIN influence map pawn

std::vector<Square> Board::influence_pawn(Square sq) const
{
    std::vector<Square> influence{};
    int front_left = 9;
    int front_right = 7;

    if (!is_left_horizontal_boundary(sq)) {
        is_white_pawn(sq) ? influence.push_back(sq + front_left) : influence.push_back(sq - front_right);
    }
    if (!is_right_horizontal_boundary(sq)) {
        is_white_pawn(sq) ? influence.push_back(sq + front_right) : influence.push_back(sq - front_left);
    }
    return influence;
}

// END influence map pawn
//----------------------------------------------------------------------------------------------------------------------
// BEGIN influence map
// can do influence map and move map simultaneously?

/**
 * @brief Creates a list of squares that a given piece is currently "influencing".
 * @param sq A given square.
 * @return A list of squares that the piece is currently "influencing".
 */
std::vector<Square> Board::influence(Square sq) const
{
    std::vector<Square> influence{};

    // if the square is empty, there is no influence
    if (what_piece(sq) == ' ') { return influence; }
    else if (is_rook(sq)) { influence = influence_rook(sq); }
    else if (is_bishop(sq)) { influence = influence_bishop(sq); }
    else if (is_queen(sq)) { influence = influence_queen(sq); }
    else if (is_knight(sq)) { influence = influence_knight(sq); }
        // TODO might be best to check for discovered checks directly from the king's perspective
        // run out in all directions
        // if there is a friendly piece "blocking" check, that friendly piece is pinned
    else if (is_king(sq)) { influence = influence_king(sq); }
    else if (is_pawn(sq)) { influence = influence_pawn(sq); }
    return influence;
}

// END influence map
//----------------------------------------------------------------------------------------------------------------------
// BEGIN legal_moves_pawn

std::vector<Square> Board::legal_moves_pawn(Square sq) const
{
    // TODO must remove diagonals if there is no enemy present
    std::vector<Square> pawn_moves{};
    if (is_white_pawn(sq)) {
        int isq = static_cast<int>(sq);
        bool starting_row = isq <= 15 && isq >= 8;
        if (is_empty(sq + 8)) { pawn_moves.push_back(sq + 8); }
        if (starting_row && is_empty(sq + 8) && is_empty(sq + 16)) { pawn_moves.push_back(sq + 16); }
    }
    if (is_black_pawn(sq)) {
        int isq = static_cast<int>(sq);
        bool starting_row = isq <= 55 && isq >= 48;
        if (is_empty(sq - 8)) { pawn_moves.push_back(sq - 8); }
        if (starting_row && is_empty(sq - 8) && is_empty(sq - 16)) { pawn_moves.push_back(sq - 16); }
    }
    return pawn_moves;
}

//----------------------------------------------------------------------------------------------------------------------
// BEGIN legal moves

// to be quick (coding-wise, not execution wise) use the influence map then go through and remove all the squares of the
// same color (can't take your own pieces)
// then, go in and add castling and promotion.
// the pawn is a special case, where it may have more moves than

std::vector<Square> Board::legal_moves(Square sq) const
{
    std::vector<Square> moves{};
    std::vector<Square> temp = influence(sq);

    if (!is_pawn(sq)) {
        for (const auto& square : temp) {
            if (!is_same_color(sq, what_color(square))) { moves.push_back(square); }
        }
    }
    if (is_pawn(sq)) {
        for (const auto& square : temp) {
            if (!is_same_color(sq, what_color(square)) && !is_empty(square)) { moves.push_back(square); }
        }
        std::vector<Square> temp1 = legal_moves_pawn(sq);
        for (const auto& square : temp1) {
            moves.push_back(square);
        }
    }
    return moves;
}

// END legal moves
//----------------------------------------------------------------------------------------------------------------------
// BEGIN update influence maps

void Board::update_influence_maps()
{
    // reset the maps
    influence_map_white.clear();
    influence_map_black.clear();

    for (Square square = Square::a8; square >= Square::h1; --square) {
        // white moves
        if (is_white(square)) {
            influence_map_white.insert({square, influence(square)});
        }
        // black moves
        if (is_black(square)) {
            influence_map_black.insert({square, influence(square)});
        }
    }
}

// END update influence maps
//----------------------------------------------------------------------------------------------------------------------
// BEGIN update king moves

void Board::update_white_king_moves(Square square_K)
{
    using s = Square;
    using c = Color;

    bool is_in_check;                           // king is in check
    bool under_attack;                          // flags a square as under attack
    std::vector<Square> influence_kings;        // temp vector to create king move list
    std::vector<Square> safe_squares;           // temp vector to create king move list
    std::vector<Square> legal_moves_kings;      // temp vector to create king move list

    // add castle squares if castling is still valid. Must be done before cleaning the list because while castling
    // ability may be present, is may not always be legal to do so, because enemy pieces could be attacking those
    // squares, or friendly pieces may be blocking them.
    // TODO if a king is in check, the only moves allowed are to 1) move out of check, 2) block check, 3) capture the
    // piece giving check

    // get started
    influence_kings = influence(square_K);

    // detect whether king is in check
    is_in_check = false;
    for (const auto& pair : influence_map_black) {
        if (std::find(pair.second.begin(), pair.second.end(), square_K) != pair.second.end()) {  // is the king in check
            is_in_check = true;
            break;
        }
    }
    if (!is_in_check) {     // if not in check, add castling moves (if available)
        if (game_state.castle_K) { influence_kings.push_back(s::g1); }
        if (game_state.castle_Q) { influence_kings.push_back(s::c1); }
    }

    // remove square that are under attack
    for (const auto& s : influence_kings) {
        under_attack = false;   // reset search flag
        for (const auto& pair : influence_map_black) {
            if (std::find(pair.second.begin(), pair.second.end(), s) != pair.second.end()) {
                under_attack = true;
                break;
            }
        }
        if (!under_attack) { safe_squares.push_back(s); }
    }
    // clean list of same color squares (cannot take own pieces)
    for (const auto& s : safe_squares) { if (!is_same_color(s, c::white)) { legal_moves_kings.push_back(s); }}

    // if the legal moves contain a castling square, but not the adjacent square necessary to legally castle
    // remove the castling square
    if (std::find(legal_moves_kings.begin(), legal_moves_kings.end(), s::g1) != legal_moves_kings.end()) {
        if (std::find(legal_moves_kings.begin(), legal_moves_kings.end(), s::f1) == legal_moves_kings.end()) {
            // remove g1
            auto it = std::remove(legal_moves_kings.begin(), legal_moves_kings.end(), s::g1);
            legal_moves_kings.erase(it, legal_moves_kings.end());
        }
    }
    if (std::find(legal_moves_kings.begin(), legal_moves_kings.end(), s::c1) != legal_moves_kings.end()) {
        if (std::find(legal_moves_kings.begin(), legal_moves_kings.end(), s::d1) == legal_moves_kings.end()) {
            // remove c1
            auto it = std::remove(legal_moves_kings.begin(), legal_moves_kings.end(), s::c1);
            legal_moves_kings.erase(it, legal_moves_kings.end());
        }
    }

    // add king moves to move map
    move_map_white.insert({square_K, legal_moves_kings});
}

void Board::update_black_king_moves(Square square_K)
{
    using s = Square;
    using c = Color;

    bool is_in_check;                           // king is in check
    bool under_attack;                          // flags a square as under attack
    std::vector<Square> influence_kings;        // temp vector to create king move list
    std::vector<Square> safe_squares;           // temp vector to create king move list
    std::vector<Square> legal_moves_kings;      // temp vector to create king move list

    // TODO if a king is in check, the only moves allowed are to 1) move out of check, 2) block check, 3) capture the
    // piece giving check

    // get started
    influence_kings = influence(square_K);

    // detect whether king is in check
    is_in_check = false;
    for (const auto& pair : influence_map_white) {
        if (std::find(pair.second.begin(), pair.second.end(), square_K) != pair.second.end()) {  // is the king in check
            is_in_check = true;
            break;
        }
    }
    if (!is_in_check) {     // if not in check, add castling moves (if available)
        if (game_state.castle_K) { influence_kings.push_back(s::g8); }
        if (game_state.castle_Q) { influence_kings.push_back(s::c8); }
    }

    // remove square that are under attack
    for (const auto& s : influence_kings) {
        under_attack = false;   // reset search flag
        for (const auto& pair : influence_map_white) {
            if (std::find(pair.second.begin(), pair.second.end(), s) != pair.second.end()) {
                under_attack = true;
                break;
            }
        }
        if (!under_attack) { safe_squares.push_back(s); }
    }
    // clean list of same color squares (cannot take own pieces)
    for (const auto& s : safe_squares) { if (!is_same_color(s, c::black)) { legal_moves_kings.push_back(s); }}

    // if the legal moves contain a castling square, but not the adjacent square necessary to legally castle
    // remove the castling square
    if (std::find(legal_moves_kings.begin(), legal_moves_kings.end(), s::g8) != legal_moves_kings.end()) {
        if (std::find(legal_moves_kings.begin(), legal_moves_kings.end(), s::f8) == legal_moves_kings.end()) {
            // remove g8
            auto it = std::remove(legal_moves_kings.begin(), legal_moves_kings.end(), s::g8);
            legal_moves_kings.erase(it, legal_moves_kings.end());
        }
    }
    if (std::find(legal_moves_kings.begin(), legal_moves_kings.end(), s::c8) != legal_moves_kings.end()) {
        if (std::find(legal_moves_kings.begin(), legal_moves_kings.end(), s::d8) == legal_moves_kings.end()) {
            // remove c1
            auto it = std::remove(legal_moves_kings.begin(), legal_moves_kings.end(), s::c8);
            legal_moves_kings.erase(it, legal_moves_kings.end());
        }
    }

    // add king moves to move map
    move_map_black.insert({square_K, legal_moves_kings});
}
// END update king moves
//----------------------------------------------------------------------------------------------------------------------
// BEGIN update move maps

Square Board::assign_from_influence_map_exclude_pawns_and_kings(std::unordered_map<Square, std::vector<Square>> *to,
        std::unordered_map<Square, std::vector<Square>> *from)
{
    Square square_king{};
    to->clear();
    for (const auto& pair : *from) {
        if (!is_pawn(pair.first) && !is_king(pair.first)) { to->insert(pair); }
        else if (is_king(pair.first)) { square_king = pair.first; }
    }
    return square_king;
}

void Board::remove_same_color_squares(std::unordered_map<Square, std::vector<Square>> *map, Color color)
{
    std::unordered_map<Square, std::vector<Square>> temp;    // to reassign original after cleaning

    for (const auto& pair : *map) {
        if (!is_same_color(pair.first, color)) { temp.insert(pair); }
    }
    *map = temp;
}

/** @warning something doesn't work correctly if there are multiple kings on the board. no prob lol.
 **/
void Board::update_move_maps()
{
    update_influence_maps();
    // assign from influence maps exclude pawns and kings
    Square square_K = assign_from_influence_map_exclude_pawns_and_kings(&move_map_white, &influence_map_white);
    Square square_k = assign_from_influence_map_exclude_pawns_and_kings(&move_map_black, &influence_map_black);

    // remove same colored squares (can't capture own pieces)
    remove_same_color_squares(&move_map_white, Color::white);
    remove_same_color_squares(&move_map_black, Color::black);

    // add pawn moves directly
    // TODO we know where the pawns are already. Can we access the uint64_t directly?
    for (auto sq = Square::a8; sq >= Square::h1; --sq) {
        if (is_white_pawn(sq)) { move_map_white.insert({sq, legal_moves(sq)}); }
        if (is_black_pawn(sq)) { move_map_black.insert({sq, legal_moves(sq)}); }
    }

    // add king moves
    update_white_king_moves(square_K);
    update_black_king_moves(square_k);
}

// END update move maps
//----------------------------------------------------------------------------------------------------------------------
void Board::print_move_map(Color color) const
{
    auto map = color == Color::white ? move_map_white : move_map_black;
    for (const auto& pair : map) {
        std::cout << static_cast<int>(pair.first) << " : ";
        for (const auto& square : pair.second) {
            std::cout << static_cast<int>(square) << " ";
        }
        std::cout << "\n";
    }
}

#endif  // SRC_BITBOARD_H_
