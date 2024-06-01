#ifndef SRC_BITBOARD_H_
#define SRC_BITBOARD_H_

#include <cstdint>
#include <string>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Square.hpp"
#include "Game_State.hpp"
#include "Move-and-rules.hpp"

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
    std::vector<uint> list_legal_moves(uint square) const;
    void move(const std::string *s);
    std::vector<Square> influence_map(Square sq) const;

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

    Move_Set move_set;
    char what_piece(Square sq) const;
    std::vector<Square> influence_map_rook(Square sq) const;
    bool is_white_rook(Square sq) const;
    bool is_black_rook(Square sq) const;
    bool is_rook(Square sq) const;
    bool is_white_bishop(Square sq) const;
    bool is_black_bishop(Square sq) const;
    bool is_bishop(Square sq) const;
    std::vector<Square> influence_map_bishop(Square sq) const;
    bool is_white_queen(Square sq) const;
    bool is_black_queen(Square sq) const;
    bool is_queen(Square sq) const;
    std::vector<Square> influence_map_queen(Square sq) const;
    bool is_white_knight(Square sq) const;
    bool is_black_knight(Square sq) const;
    bool is_knight(Square sq) const;
    std::vector<Square> influence_map_knight(Square sq) const;
    bool is_white_king(Square sq) const;
    bool is_black_king(Square sq) const;
    bool is_king(Square sq) const;
    std::vector<Square> influence_map_king(Square sq) const;
    bool is_white_pawn(Square sq) const;
    bool is_black_pawn(Square sq) const;
    bool is_pawn(Square sq) const;
    std::vector<Square> influence_map_pawn(Square sq) const;
    bool is_under_attack(Square sq, Color color) const;
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

//----------------------------------------------------------------------------------------------------------------------
// BEGIN piece detection

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
// BEGIN boundary checking

bool is_upper_vertical_boundary(Square square)
{
    return static_cast<int>(square) > 55;
}

bool is_lower_vertical_boundary(Square square)
{
    return static_cast<int>(square) < 8;
}

bool is_vertical_boundary(Square sq)
{
    return is_upper_vertical_boundary(sq) || is_lower_vertical_boundary(sq);
}

bool is_left_horizontal_boundary(Square sq)
{
    using s = Square;
    return sq == s::a1 || sq == s::a2 || sq == s::a3 || sq == s::a4 || sq == s::a5 || sq == s::a6 || sq == s::a7
            || sq == s::a8;
}

bool is_right_horizontal_boundary(Square sq)
{
    using s = Square;
    return sq == s::h1 || sq == s::h2 || sq == s::h3 || sq == s::h4 || sq == s::h5 || sq == s::h6 || sq == s::h7
            || sq == s::h8;
}

bool is_horizontal_boundary(Square sq)
{
    return is_left_horizontal_boundary(sq) || is_right_horizontal_boundary(sq);
}

bool is_boundary(Square sq)
{
    return is_vertical_boundary(sq) || is_horizontal_boundary(sq);
}

bool is_corner(Square sq)
{
    return is_vertical_boundary(sq) && is_horizontal_boundary(sq);
}

// END boundary checking
//----------------------------------------------------------------------------------------------------------------------
// BEGIN influence map rook

std::vector<Square> Board::influence_map_rook(Square sq) const
{
    std::vector<Square> influence_map{};
    // vertical up
    for (auto square = sq + 8; !is_upper_vertical_boundary(square - 8); square = square + 8) {
        influence_map.push_back(square);
        if (what_piece(square) != ' ') { break; }
    }
    // vertical down
    for (auto square = sq - 8; !is_lower_vertical_boundary(square + 8); square = square - 8) {
        influence_map.push_back(square);
        if (what_piece(square) != ' ') { break; }
    }
    // horizontal right
    for (auto square = sq - 1; !is_right_horizontal_boundary(square + 1); square = square - 1) {
        influence_map.push_back(square);
        if (what_piece(square) != ' ') { break; }
    }
    // horizontal left
    for (auto square = sq + 1; !is_left_horizontal_boundary(square - 1); square = square + 1) {
        influence_map.push_back(square);
        if (what_piece(square) != ' ') { break; }
    }
    return influence_map;
}

// END influence map rook
//----------------------------------------------------------------------------------------------------------------------
// BEGIN influence map bishop

std::vector<Square> Board::influence_map_bishop(Square sq) const
{
    std::vector<Square> influence_map{};
    // up left
    for (auto square = sq + 9;
         !is_upper_vertical_boundary(square - 9) && !is_left_horizontal_boundary(square - 9);
         square = square + 9) {
        influence_map.push_back(square);
        if (what_piece(square) != ' ') { break; }
    }
    // up right
    for (auto square = sq + 7;
         !is_upper_vertical_boundary(square - 7) && !is_right_horizontal_boundary(square - 7);
         square = square + 7) {
        influence_map.push_back(square);
        if (what_piece(square) != ' ') { break; }
    }
    // down right
    for (auto square = sq - 9;
         !is_lower_vertical_boundary(square + 9) && !is_right_horizontal_boundary(square + 9);
         square = square - 9) {
        influence_map.push_back(square);
        if (what_piece(square) != ' ') { break; }
    }
    // down left
    for (auto square = sq - 7;
         !is_lower_vertical_boundary(square + 7) && !is_left_horizontal_boundary(square + 7);
         square = square - 7) {
        influence_map.push_back(square);
        if (what_piece(square) != ' ') { break; }
    }
    return influence_map;
}

// END influence map bishop
//----------------------------------------------------------------------------------------------------------------------
// BEGIN influence map queen

std::vector<Square> Board::influence_map_queen(Square sq) const
{
    std::vector<Square> v1 = influence_map_rook(sq);
    std::vector<Square> v2 = influence_map_bishop(sq);

    v1.insert(v1.end(), v2.begin(), v2.end());

    return v1;
}

// END influence map queen
//----------------------------------------------------------------------------------------------------------------------
// BEGIN influence map knight

std::vector<Square> Board::influence_map_knight(Square sq) const
{
    using s = Square;
    std::vector<Square> influence_map{};
    int isq = static_cast<int>(sq);

    // center 4x4 - 16 squares
    if (isq >= 18 && isq <= 21 || isq >= 26 && isq <= 29 || isq >= 34 && isq <= 37 || isq >= 42 && isq <= 45) {
        influence_map.push_back(sq - (2 * 8 + 1));     // down right
        influence_map.push_back(sq - (2 * 8 - 1));     // down left
        influence_map.push_back(sq - (8 + 2));         // right down
        influence_map.push_back(sq - (8 - 2));         // left down
        influence_map.push_back(sq + (8 - 2));         // right up
        influence_map.push_back(sq + (8 + 2));         // left up
        influence_map.push_back(sq + (2 * 8 - 1));     // up right
        influence_map.push_back(sq + (2 * 8 + 1));     // up left
    }
        // b file - 4 squares
    else if (sq == Square::b3 || sq == Square::b4 || sq == Square::b5 || sq == Square::b6) {
        influence_map.push_back(sq - (2 * 8 + 1));     // down right
        influence_map.push_back(sq - (2 * 8 - 1));     // down left
        influence_map.push_back(sq - (8 + 2));         // right down
        influence_map.push_back(sq + (8 - 2));         // right up
        influence_map.push_back(sq + (2 * 8 - 1));     // up right
        influence_map.push_back(sq + (2 * 8 + 1));     // up left
    }
        // g files - 4 squares
    else if (sq == Square::g3 || sq == Square::g4 || sq == Square::g5 || sq == Square::g6) {
        influence_map.push_back(sq - (2 * 8 + 1));     // down right
        influence_map.push_back(sq - (2 * 8 - 1));     // down left
        influence_map.push_back(sq - (8 - 2));         // left down
        influence_map.push_back(sq + (8 + 2));         // left up
        influence_map.push_back(sq + (2 * 8 - 1));     // up right
        influence_map.push_back(sq + (2 * 8 + 1));     // up left
    }
        // 2nd rank - 4 squares
    else if (isq >= static_cast<int>(Square::f2) && isq <= static_cast<int>(Square::c2)) {
        influence_map.push_back(sq - (8 + 2));         // right down
        influence_map.push_back(sq - (8 - 2));         // left down
        influence_map.push_back(sq + (8 - 2));         // right up
        influence_map.push_back(sq + (8 + 2));         // left up
        influence_map.push_back(sq + (2 * 8 - 1));     // up right
        influence_map.push_back(sq + (2 * 8 + 1));     // up left
    }
        // 7th rank - 4 squares
    else if (isq >= static_cast<int>(Square::f7) && isq <= static_cast<int>(Square::c7)) {
        influence_map.push_back(sq - (2 * 8 + 1));     // down right
        influence_map.push_back(sq - (2 * 8 - 1));     // down left
        influence_map.push_back(sq - (8 + 2));         // right down
        influence_map.push_back(sq - (8 - 2));         // left down
        influence_map.push_back(sq + (8 - 2));         // right up
        influence_map.push_back(sq + (8 + 2));         // left up
    }
        // a file - 4 squares
    else if (sq == Square::a3 || sq == Square::a4 || sq == Square::a5 || sq == Square::a6) {
        influence_map.push_back(sq - (2 * 8 + 1));     // down right
        influence_map.push_back(sq - (8 + 2));         // right down
        influence_map.push_back(sq + (8 - 2));         // right up
        influence_map.push_back(sq + (2 * 8 - 1));     // up right
    }
        // h file - 4 squares
    else if (sq == Square::h3 || sq == Square::h4 || sq == Square::h5 || sq == Square::h6) {
        influence_map.push_back(sq - (2 * 8 - 1));     // down left
        influence_map.push_back(sq - (8 - 2));         // left down
        influence_map.push_back(sq + (8 + 2));         // left up
        influence_map.push_back(sq + (2 * 8 + 1));     // up left
    }
        // 1st rank - 4 squares
    else if (isq >= static_cast<int>(Square::f1) && isq <= static_cast<int>(Square::c1)) {
        influence_map.push_back(sq + (8 - 2));         // right up
        influence_map.push_back(sq + (8 + 2));         // left up
        influence_map.push_back(sq + (2 * 8 - 1));     // up right
        influence_map.push_back(sq + (2 * 8 + 1));     // up left
    }
        // 8th rank - 4 squares
    else if (isq >= static_cast<int>(Square::f8) && isq <= static_cast<int>(Square::c8)) {
        influence_map.push_back(sq - (2 * 8 + 1));     // down right
        influence_map.push_back(sq - (2 * 8 - 1));     // down left
        influence_map.push_back(sq - (8 + 2));         // right down
        influence_map.push_back(sq - (8 - 2));         // left down
    }
        // inside 2's and 7's - 1 square each
    else if (isq == static_cast<int>(Square::g2)) { influence_map = {s::e1, s::e3, s::h4, s::f4}; }
    else if (isq == static_cast<int>(Square::b2)) { influence_map = {s::d1, s::d3, s::c4, s::a4}; }
    else if (isq == static_cast<int>(Square::g7)) { influence_map = {s::h5, s::f5, s::e6, s::e8}; }
    else if (isq == static_cast<int>(Square::b7)) { influence_map = {s::c5, s::a5, s::d6, s::d8}; }
        // edge 2's and 7's - 1 square each
    else if (isq == static_cast<int>(Square::h2)) { influence_map = {s::f1, s::f3, s::g4}; }
    else if (isq == static_cast<int>(Square::a2)) { influence_map = {s::c1, s::c3, s::b4}; }
    else if (isq == static_cast<int>(Square::h7)) { influence_map = {s::g5, s::f6, s::f8}; }
    else if (isq == static_cast<int>(Square::a7)) { influence_map = {s::b5, s::c6, s::c8}; }
        // edge 1's and 8's - 1 square each
    else if (isq == static_cast<int>(Square::g1)) { influence_map = {s::e2, s::h3, s::f3}; }
    else if (isq == static_cast<int>(Square::b1)) { influence_map = {s::d2, s::c3, s::a3}; }
    else if (isq == static_cast<int>(Square::g8)) { influence_map = {s::h6, s::f6, s::e7}; }
    else if (isq == static_cast<int>(Square::b8)) { influence_map = {s::c6, s::a6, s::d7}; }
        // corners - 1 square each
    else if (isq == static_cast<int>(Square::h1)) { influence_map = {s::f2, s::g3}; }
    else if (isq == static_cast<int>(Square::a1)) { influence_map = {s::c2, s::b3}; }
    else if (isq == static_cast<int>(Square::h8)) { influence_map = {s::g6, s::f7}; }
    else if (isq == static_cast<int>(Square::a8)) { influence_map = {s::b6, s::c7}; }

    return influence_map;
}

// END influence map knight
//----------------------------------------------------------------------------------------------------------------------
// BEGIN influence map king

std::vector<Square> Board::influence_map_king(Square sq) const
{
    std::vector<Square> influence_map{};

    // vertical
    if (!is_upper_vertical_boundary(sq)) { influence_map.push_back(sq + 8); }
    if (!is_lower_vertical_boundary(sq)) { influence_map.push_back(sq - 8); }
    // horizontal
    if (!is_right_horizontal_boundary(sq)) { influence_map.push_back(sq - 1); }
    if (!is_left_horizontal_boundary(sq)) { influence_map.push_back(sq + 1); }
    // diagonal
    if (!is_lower_vertical_boundary(sq) && !is_right_horizontal_boundary(sq)) { influence_map.push_back(sq - 9); }
    if (!is_lower_vertical_boundary(sq) && !is_left_horizontal_boundary(sq)) { influence_map.push_back(sq - 7); }
    if (!is_upper_vertical_boundary(sq) && !is_right_horizontal_boundary(sq)) { influence_map.push_back(sq + 7); }
    if (!is_upper_vertical_boundary(sq) && !is_left_horizontal_boundary(sq)) { influence_map.push_back(sq + 9); }

    return influence_map;
}

// END influence map king
//----------------------------------------------------------------------------------------------------------------------
// BEGIN influence map pawn

std::vector<Square> Board::influence_map_pawn(Square sq) const
{
    std::vector<Square> influence_map{};
    int front_left = 9;
    int front_right = 7;

    if (!is_left_horizontal_boundary(sq)) {
        is_white_pawn(sq) ? influence_map.push_back(sq + front_left) : influence_map.push_back(sq - front_right);
    }
    if (!is_right_horizontal_boundary(sq)) {
        is_white_pawn(sq) ? influence_map.push_back(sq + front_right) : influence_map.push_back(sq - front_left);
    }
    return influence_map;
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
std::vector<Square> Board::influence_map(Square sq) const
{
    std::vector<Square> influence_map{};

    // if the square is empty, there is no influence
    if (what_piece(sq) == ' ') { return influence_map; }
    else if (is_rook(sq)) { influence_map = influence_map_rook(sq); }
    else if (is_bishop(sq)) { influence_map = influence_map_bishop(sq); }
    else if (is_queen(sq)) { influence_map = influence_map_queen(sq); }
    else if (is_knight(sq)) { influence_map = influence_map_knight(sq); }
        // TODO might be best to check for discovered checks directly from the king's perspective
        // run out in all directions
        // if there is a friendly piece "blocking" check, that friendly piece is pinned
    else if (is_king(sq)) { influence_map = influence_map_king(sq); }
    else if (is_pawn(sq)) { influence_map = influence_map_pawn(sq); }
    return influence_map;
}

// END influence map
//----------------------------------------------------------------------------------------------------------------------
// BEGIN is under attack

/**
 * @brief Determines if a given square is under attack.
 * @param sq The square to check.
 * @param color The color of the attacking pieces.
 * @return True if the square is under attack, false otherwise.
 */
bool Board::is_under_attack(Square sq, Color color) const
{
    // check vertical, horizontal, diagonal, and knight L's
    // check if the square is occupied by the appropriate enemy piece
    // or if the lane is "blocked" by a non-threatening enemy piece, or any friendly piece
    // or if the lane is empty
    // break and return true if the square is under attack
    // otherwise, return false

    // vertical attacks may come from a rook, queen, or king
    // horizontal attacks may come from a rook, queen, or king
    // diagonal attacks may come from a bishop, queen, pawn, or king
    // knight attacks may come from a knight

    // vertical up
    bool first_iteration = true;
    for (auto square = sq + 8; !is_upper_vertical_boundary(square - 8); square = square + 8) {
        if (first_iteration) {
            if (is_opposite_king(square, color)) { return true; }
            first_iteration = false;
        }
        if (is_opposite_rook(square, color) || is_opposite_queen(square, color)) { return true; }
        else if (is_same_color(square, color) || is_opposite_pawn(square, color)
                || is_opposite_bishop(square, color) || is_opposite_knight(square, color)) {
            break;
        }
    }
    // vertical down
    first_iteration = true;
    for (auto square = sq - 8; !is_lower_vertical_boundary(square + 8); square = square - 8) {
        if (first_iteration) {
            if (is_opposite_king(square, color)) { return true; }
            first_iteration = false;
        }
        if (is_opposite_rook(square, color) || is_opposite_queen(square, color)) { return true; }
        else if (is_same_color(square, color) || is_opposite_pawn(square, color)
                || is_opposite_bishop(square, color) || is_opposite_knight(square, color)) {
            break;
        }
    }
    // horizontal left
    first_iteration = true;
    for (auto square = sq + 1; !is_left_horizontal_boundary(square - 1); square = square + 1) {
        if (first_iteration) {
            if (is_opposite_king(square, color)) { return true; }
            first_iteration = false;
        }
        if (is_opposite_rook(square, color) || is_opposite_queen(square, color)) { return true; }
        else if (is_same_color(square, color) || is_opposite_pawn(square, color)
                || is_opposite_bishop(square, color) || is_opposite_knight(square, color)) {
            break;
        }
    }
    // horizontal right
    first_iteration = true;
    for (auto square = sq - 1; !is_right_horizontal_boundary(square + 1); square = square - 1) {
        if (first_iteration) {
            if (is_opposite_king(square, color)) { return true; }
            first_iteration = false;
        }
        if (is_opposite_rook(square, color) || is_opposite_queen(square, color)) { return true; }
        else if (is_same_color(square, color) || is_opposite_pawn(square, color)
                || is_opposite_bishop(square, color) || is_opposite_knight(square, color)) {
            break;
        }
    }
    // diagonal down right
    first_iteration = true;
    for (auto square = sq - 9;
         !is_right_horizontal_boundary(square + 9) && !is_lower_vertical_boundary(square + 9);
         square = square - 9) {
        if (first_iteration) {
            if (is_opposite_king(square, color)) { return true; }
            if (color == Color::black) { return is_white_pawn(square); }
            first_iteration = false;
        }
        if (is_opposite_bishop(square, color) || is_opposite_queen(square, color)) { return true; }
        else if (is_same_color(square, color) || is_opposite_pawn(square, color)
                || is_opposite_rook(square, color) || is_opposite_knight(square, color)) {
            break;
        }
    }
    // diagonal down left
    // TODO really can't figure out why this won't work ...
    // Branch coverage shows it's not even hit in the tests ...
    // TODO until this gets fixed, I suppose I could just make the influence map then check to see if the king is in it
    // while probably slower, it should work.
    first_iteration = true;
    for (auto square = sq - 7;
         !is_left_horizontal_boundary(square + 7) && !is_lower_vertical_boundary(square + 7);
         square = square - 7) {
        if (first_iteration) {
            if (is_opposite_king(square, color)) { return true; }
            if (color == Color::black) { return is_white_pawn(square); }
            first_iteration = false;
        }
        if (is_opposite_bishop(square, color) || is_opposite_queen(square, color)) { return true; }
        else if (is_same_color(square, color) || is_opposite_pawn(square, color)
                || is_opposite_rook(square, color) || is_opposite_knight(square, color)) {
            break;
        }
    }
    // diagonal up right

    // diagonal up left

    return false;
}

// END is under attack
//----------------------------------------------------------------------------------------------------------------------

// scratch

std::vector<uint> Board::list_legal_moves(uint square) const
{
    std::vector<uint> legal_moves{};

    // first we find what piece is in that square
    char piece = what_piece(square);

    // if the square is empty, there are no moves to make.
    if (piece == ' ') { return legal_moves; }

    // maybe we should start with a check detector ...
    // we could make a control map sort of thing ... a list of each piece and the squares it is "influencing"
    // then check if the king is there, for discovered checks, etc.

    if (piece == 'P') {
        // white pawn
        // is there a piece in front of it?
        // move forward one: current square +/- 8
        // move forward two - must record en passant target: current square +/- 8*2
        // capture left/right - a piece must occupy the square in question
        // promote: when entering extreme opposite rank, could promote to any other piece
    }
}

void Board::move(const std::string *s)
{
    // check to see what kind of piece is in the start square
    // make a list of all available moves for that piece
    // if the given move is legal, move the piece, update game state
    uint from_sq = square_to_uint(s->substr(0, 2));
    uint to_sq = square_to_uint(s->substr(2, 2));
    list_legal_moves(from_sq);
}

#endif  // SRC_BITBOARD_H_
