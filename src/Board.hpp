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
    std::vector<Square> attack_map(Square sq) const;

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

bool is_vertical_boundary(Square sq)
{
    return static_cast<int>(sq) < 8 || static_cast<int>(sq) > 55;

}

bool is_horizontal_boundary(Square sq)
{
    return static_cast<int>(sq) % 8 == 0 || (static_cast<int>(sq) + 1) % 8 == 0;
}

bool is_boundary(Square sq)
{
    return is_vertical_boundary(sq) || is_horizontal_boundary(sq);
}

// can do attack map and move map simultaneously?
std::vector<Square> Board::attack_map(Square sq) const
{
    std::vector<Square> attack_map{};

    // if the square is empty, there is no influence

    if (what_piece(sq) == ' ') { return attack_map; }
    else if (what_piece(sq) == 'R' || what_piece(sq) == 'r') {
        // check all the rookie things to do ...
        // check vertical with bounds
        for (auto square = sq + 8; !is_vertical_boundary(square - 8); square = square + 8) {
            if (what_piece(square) == ' ') {
                attack_map.push_back(square);
            }
            else {
                attack_map.push_back(square);
                break;
            }
        }
        for (auto square = sq - 8; !is_vertical_boundary(square + 8); square = square - 8) {
            if (what_piece(square) == ' ') {
                attack_map.push_back(square);
            }
            else {
                attack_map.push_back(square);
                break;
            }
        }
        // check horizontal with bounds
        for (auto square = sq + 1; !is_horizontal_boundary(square - 1); square = square + 1) {
            if (what_piece(square) == ' ') {
                attack_map.push_back(square);
            }
            else {
                attack_map.push_back(square);
                break;
            }
        }
        for (auto square = sq - 1; !is_horizontal_boundary(square + 1); square = square - 1) {
            if (what_piece(square) == ' ') {
                attack_map.push_back(square);
            }
            else {
                attack_map.push_back(square);
                break;
            }
        }
    }
    else if (what_piece(sq) == 'B' || what_piece(sq) == 'b') {
        // check all the bishopy things to do
        // up right
        for (auto square = sq + 9; !is_boundary(square - 9); square = square + 9) {
            if (what_piece(square) == ' ') {
                attack_map.push_back(square);
            }
            else {
                attack_map.push_back(square);
                break;
            }
        }
        // up left
        for (auto square = sq + 7; !is_boundary(square - 7); square = square + 7) {
            if (what_piece(square) == ' ') {
                attack_map.push_back(square);
            }
            else {
                attack_map.push_back(square);
                break;
            }
        }
        // down right
        for (auto square = sq - 7; !is_boundary(square + 7); square = square - 7) {
            if (what_piece(square) == ' ') {
                attack_map.push_back(square);
            }
            else {
                attack_map.push_back(square);
                break;
            }
        }
        // down left
        for (auto square = sq - 9; !is_boundary(square + 9); square = square - 9) {
            if (what_piece(square) == ' ') {
                attack_map.push_back(square);
            }
            else {
                attack_map.push_back(square);
                break;
            }
        }
    }
    else if (what_piece(sq) == 'Q' || what_piece(sq) == 'q') {
        for (auto square = sq + 8; !is_vertical_boundary(square - 8); square = square + 8) {
            if (what_piece(square) == ' ') {
                attack_map.push_back(square);
            }
            else {
                attack_map.push_back(square);
                break;
            }
        }
        for (auto square = sq - 8; !is_vertical_boundary(square + 8); square = square - 8) {
            if (what_piece(square) == ' ') {
                attack_map.push_back(square);
            }
            else {
                attack_map.push_back(square);
                break;
            }
        }
        // check horizontal with bounds
        for (auto square = sq + 1; !is_horizontal_boundary(square - 1); square = square + 1) {
            if (what_piece(square) == ' ') {
                attack_map.push_back(square);
            }
            else {
                attack_map.push_back(square);
                break;
            }
        }
        for (auto square = sq - 1; !is_horizontal_boundary(square + 1); square = square - 1) {
            if (what_piece(square) == ' ') {
                attack_map.push_back(square);
            }
            else {
                attack_map.push_back(square);
                break;
            }
        }
        // up right
        for (auto square = sq + 9; !is_boundary(square - 9); square = square + 9) {
            if (what_piece(square) == ' ') {
                attack_map.push_back(square);
            }
            else {
                attack_map.push_back(square);
                break;
            }
        }
        // up left
        for (auto square = sq + 7; !is_boundary(square - 7); square = square + 7) {
            if (what_piece(square) == ' ') {
                attack_map.push_back(square);
            }
            else {
                attack_map.push_back(square);
                break;
            }
        }
        // down right
        for (auto square = sq - 7; !is_boundary(square + 7); square = square - 7) {
            if (what_piece(square) == ' ') {
                attack_map.push_back(square);
            }
            else {
                attack_map.push_back(square);
                break;
            }
        }
        // down left
        for (auto square = sq - 9; !is_boundary(square + 9); square = square - 9) {
            if (what_piece(square) == ' ') {
                attack_map.push_back(square);
            }
            else {
                attack_map.push_back(square);
                break;
            }
        }
    }
    else if (what_piece(sq) == 'N' || what_piece(sq) == 'n') {
        // center 4x4
        int isq = static_cast<int>(sq);
        if (isq >= 18 && isq <= 21 || isq >= 26 && isq <= 29 || isq >= 34 && isq <= 37 || isq >= 42 && isq <= 45) {
            attack_map.push_back(sq + (2 * 8 - 1));     // up right
            attack_map.push_back(sq + (2 * 8 + 1));     // up left
            attack_map.push_back(sq + (8 - 2));         // right up
            attack_map.push_back(sq + (8 + 2));         // left up
            attack_map.push_back(sq - (8 + 2));         // right down
            attack_map.push_back(sq - (8 - 2));         // left down
            attack_map.push_back(sq - (2 * 8 + 1));     // down right
            attack_map.push_back(sq - (2 * 8 - 1));     // down left
        }
        else if (isq > 14 && isq < 54 && (isq + 2) % 8 == 0) {
            attack_map.push_back(sq + (2 * 8 - 1));     // up right
            attack_map.push_back(sq + (2 * 8 + 1));     // up left
            attack_map.push_back(sq + (8 - 2));         // right up
            attack_map.push_back(sq - (8 + 2));         // right down
            attack_map.push_back(sq - (2 * 8 + 1));     // down right
            attack_map.push_back(sq - (2 * 8 - 1));     // down left
        }
        else if (isq > 9 && isq < 49 && (isq - 1) % 8 == 0) {
            attack_map.push_back(sq + (2 * 8 - 1));     // up right
            attack_map.push_back(sq + (2 * 8 + 1));     // up left
            attack_map.push_back(sq + (8 + 2));         // left up
            attack_map.push_back(sq - (8 - 2));         // left down
            attack_map.push_back(sq - (2 * 8 + 1));     // down right
            attack_map.push_back(sq - (2 * 8 - 1));     // down left
        }
        else if (isq >= 50 && isq <= 53) {
            attack_map.push_back(sq + (8 - 2));         // right up
            attack_map.push_back(sq + (8 + 2));         // left up
            attack_map.push_back(sq - (8 + 2));         // right down
            attack_map.push_back(sq - (8 - 2));         // left down
            attack_map.push_back(sq - (2 * 8 + 1));     // down right
            attack_map.push_back(sq - (2 * 8 - 1));     // down left
        }
        else if (isq >= 10 && isq <= 13) {
            attack_map.push_back(sq + (2 * 8 - 1));     // up right
            attack_map.push_back(sq + (2 * 8 + 1));     // up left
            attack_map.push_back(sq + (8 - 2));         // right up
            attack_map.push_back(sq + (8 + 2));         // left up
            attack_map.push_back(sq - (8 + 2));         // right down
            attack_map.push_back(sq - (8 - 2));         // left down
        }
        else if (isq == 9) {
            attack_map.push_back(sq + (2 * 8 - 1));     // up right
            attack_map.push_back(sq + (2 * 8 + 1));     // up left
            attack_map.push_back(sq + (8 + 2));         // left up
            attack_map.push_back(sq - (8 - 2));         // left down
        }
        else if (isq == static_cast<int>(Square::b2)) {
            attack_map.push_back(sq + (2 * 8 - 1));     // up right
            attack_map.push_back(sq + (2 * 8 + 1));     // up left
            attack_map.push_back(sq + (8 - 2));         // right up
            attack_map.push_back(sq - (8 + 2));         // right down
        }
        else if (isq == static_cast<int>(Square::g7)) {
            attack_map.push_back(sq + (8 + 2));         // left up
            attack_map.push_back(sq - (8 - 2));         // left down
            attack_map.push_back(sq - (2 * 8 + 1));     // down right
            attack_map.push_back(sq - (2 * 8 - 1));     // down left
        }
        else if (isq == static_cast<int>(Square::b7)) {
            attack_map.push_back(sq + (8 - 2));         // right up
            attack_map.push_back(sq - (8 + 2));         // right down
            attack_map.push_back(sq - (2 * 8 + 1));     // down right
            attack_map.push_back(sq - (2 * 8 - 1));     // down left
        }
        else if (isq == static_cast<int>(Square::g1)) {
            attack_map.push_back(sq + (2 * 8 - 1));     // up right
            attack_map.push_back(sq + (2 * 8 + 1));     // up left
            attack_map.push_back(sq + (8 + 2));         // left up
        }
        else if (isq == static_cast<int>(Square::b1)) {
            attack_map.push_back(sq + (2 * 8 - 1));     // up right
            attack_map.push_back(sq + (2 * 8 + 1));     // up left
            attack_map.push_back(sq + (8 - 2));         // right up
        }
        else if (isq == static_cast<int>(Square::h2)) {
            attack_map.push_back(sq + (2 * 8 + 1));     // up left
            attack_map.push_back(sq + (8 + 2));         // left up
            attack_map.push_back(sq - (8 - 2));         // left down
        }
        else if (isq == static_cast<int>(Square::a2)) {
            attack_map.push_back(sq + (2 * 8 - 1));     // up right
            attack_map.push_back(sq + (8 - 2));         // right up
            attack_map.push_back(sq - (8 + 2));         // right down
        }
        else if (isq == static_cast<int>(Square::h7)) {
            attack_map.push_back(sq + (8 + 2));         // left up
            attack_map.push_back(sq - (8 - 2));         // left down
            attack_map.push_back(sq - (2 * 8 - 1));     // down left
        }
        else if (isq == static_cast<int>(Square::a7)) {
            attack_map.push_back(sq + (8 - 2));         // right up
            attack_map.push_back(sq - (8 + 2));         // right down
            attack_map.push_back(sq - (2 * 8 + 1));     // down right
        }
        else if (isq == static_cast<int>(Square::g8)) {
            attack_map.push_back(sq - (8 - 2));         // left down
            attack_map.push_back(sq - (2 * 8 + 1));     // down right
            attack_map.push_back(sq - (2 * 8 - 1));     // down left
        }
        else if (isq == static_cast<int>(Square::b8)) {
            attack_map.push_back(sq - (8 + 2));         // right down
            attack_map.push_back(sq - (2 * 8 + 1));     // down right
            attack_map.push_back(sq - (2 * 8 - 1));     // down left
        }
            // h1, a1, h8, a8
        else if (isq == static_cast<int>(Square::h1)) {
            attack_map.push_back(sq + (2 * 8 + 1));     // up left
            attack_map.push_back(sq + (8 + 2));         // left up
        }
        else if (isq == static_cast<int>(Square::a1)) {
            attack_map.push_back(sq + (2 * 8 - 1));     // up right
            attack_map.push_back(sq + (8 - 2));         // right up
        }
        else if (isq == static_cast<int>(Square::h8)) {
            attack_map.push_back(sq - (8 - 2));         // left down
            attack_map.push_back(sq - (2 * 8 - 1));     // down left
        }
        else if (isq == static_cast<int>(Square::a8)) {
            attack_map.push_back(sq - (8 + 2));         // right down
            attack_map.push_back(sq - (2 * 8 + 1));     // down right
        }
    }
    return attack_map;
}

// scratch
//----------------------------------------------------------------------------------------------------------------------
std::vector<uint> Board::list_legal_moves(uint square) const
{
    std::vector<uint> legal_moves{};

    // first we find what piece is in that square
    char piece = what_piece(square);

    // if the square is empty, there are no moves to make.
    if (piece == ' ') { return legal_moves; }

    // maybe we should start with a check detector ...
    // we could make a control map sort of thing ... a list of each piece and the squares it is "attacking"
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
    // if the given move is legal, move the piece, update gamestate
    uint from_sq = square_to_uint(s->substr(0, 2));
    uint to_sq = square_to_uint(s->substr(2, 2));
    list_legal_moves(from_sq);
}

#endif  // SRC_BITBOARD_H_
