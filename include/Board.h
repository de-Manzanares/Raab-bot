#ifndef INCLUDE_BOARD_H_
#define INCLUDE_BOARD_H_

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Square.h"
#include "Game_State.h"

/**
 * @struct Diagonals
 * @brief Groups the diagonals of the board for answering the question "are these squares in the same diagonal?"
 */
struct Diagonals {
    using s = Square;
    static std::vector<std::vector<Square>> left_right;
    static std::vector<std::vector<Square>> right_left;
};

/**
 * @class Maps
 * @brief A collection of unordered maps used for move generation
 */
struct Maps {
    std::unordered_map<Square, std::vector<Square>> white_moves{};               // all white moves
    std::unordered_map<Square, std::vector<Square>> black_moves{};               // all black moves
    std::unordered_map<Square, std::vector<Square>> white_influence{};          // all white influence
    std::unordered_map<Square, std::vector<Square>> black_influence{};          // all black influence
    std::unordered_map<Square, Square> white_pinned{};                       // {pinned piece, pinning piece}
    std::unordered_map<Square, Square> black_pinned{};                       // {pinned piece, pinning piece}
    std::unordered_map<Square, std::vector<Square>> white_pinned_lane{};  // {square, squares in pinned lane}
    std::unordered_map<Square, std::vector<Square>> black_pinned_lane{};  // {square, squares in pinned lane}
    void clear();                                                                   // empty the maps
};

/**
 * @brief Represents the chessboard
 * @details Stores all relevant game-state data and enforces rules during move generation
 */
struct Board {
    ~Board();
    Board& operator=(const Board& rhs);
    // bitboards
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

    Game_State game_state;  // game conditions aside from piece placement

    Maps *maps = new Maps;  // for move generation

    // map character code to bitboard
    std::unordered_map<char, uint64_t&> piece_map = {
            {'p', b_pawn}, {'n', b_night}, {'b', b_bishop}, {'r', b_rook}, {'q', b_queen}, {'k', b_king},
            {'P', w_Pawn}, {'N', w_Night}, {'B', w_Bishop}, {'R', w_Rook}, {'Q', w_Queen}, {'K', w_King}};

    // utility
    void clear();

    // bitwise movement
    void place_piece(Square square, const char& ch);

    // piece detection
    char what_piece(Square sq) const;
    char what_piece(uint sq) const;
    bool is_white_rook(Square sq) const;
    bool is_black_rook(Square sq) const;
    bool is_rook(Square sq) const;
    bool is_white_bishop(Square sq) const;
    bool is_black_bishop(Square sq) const;
    bool is_bishop(Square sq) const;
    bool is_white_queen(Square sq) const;
    bool is_black_queen(Square sq) const;
    bool is_queen(Square sq) const;
    bool is_white_knight(Square sq) const;
    bool is_black_knight(Square sq) const;
    bool is_knight(Square sq) const;
    bool is_white_king(Square sq) const;
    bool is_black_king(Square sq) const;
    bool is_king(Square sq) const;
    bool is_white_pawn(Square sq) const;
    bool is_black_pawn(Square sq) const;
    bool is_pawn(Square sq) const;
    bool is_black(Square sq) const;
    bool is_white(Square sq) const;
    bool is_empty(Square sq) const;
    Color what_color(Square sq) const;
    bool is_same_color(Square sq, Color color) const;
    bool is_opposite_king(Square sq, Color c) const;
    int is_in_row(Square sq);
    int is_in_row(Square sq) const;
    int is_in_column(Square sq);
    bool is_in_same_row(Square sq1, Square sq2);
    bool is_in_same_column(Square sq1, Square sq2);
    static bool is_in_same_diagonal_left_right(Square sq1, Square sq2);
    static bool is_in_same_diagonal_right_left(Square sq1, Square sq2);
    bool is_in_same_row(Square sq1, Square sq2, Square sq3);
    bool is_in_same_column(Square sq1, Square sq2, Square sq3);
    static bool is_in_same_diagonal_left_right(Square sq1, Square sq2, Square sq3);
    static bool is_in_same_diagonal_right_left(Square sq1, Square sq2, Square sq3);

    // fen
    // fen out
    std::string fen_piece_placement() const;
    std::string export_fen() const;
    // fen in
    uint set_pieces(const std::string& fen);
    void import_fen(const std::string& fen);

    // begin move generation     ----------------------------------------
    // auxiliary // TODO rename and trim these two functions
    std::vector<Square> legal_moves_pawn(Square sq) const;
    std::vector<Square> legal_moves(Square sq);

    // influence
    std::vector<Square> influence_rook(Square sq) const;
    std::vector<Square> influence_bishop(Square sq) const;
    std::vector<Square> influence_queen(Square sq) const;
    static std::vector<Square> influence_knight(Square sq);
    static std::vector<Square> influence_king(Square sq);
    std::vector<Square> influence_pawn(Square sq) const;
    std::vector<Square> influence(Square sq) const;;
    void update_influence_maps() const;

    // pinned pieces
    Square pinned_piece_rook(Square sq) const;
    Square pinned_piece_bishop(Square sq) const;
    Square pinned_piece_queen(Square sq) const;
    Square pinned_piece(Square sq) const;
    void update_pinned_pieces(const Square& sq_w_King, const Square& sq_b_king) const;

    // influence reduction
    std::vector<Square> update_white_king_moves(Square sq_w_king);
    std::vector<Square> update_black_king_moves(Square sq_b_king);
    Square copy_influence(const Color c, std::unordered_map<Square, std::vector<Square>> *from,
            std::unordered_map<Square, std::vector<Square>> *to) const;
    void remove_same_color_squares(Color color, std::unordered_map<Square, std::vector<Square>> *map) const;
    void update_move_maps();
    // end move generation     ----------------------------------------

    // uci

    // diagnostic
    ulong nodes_at_depth_1(Color color);
    void move(Square from, Square to, char ch);
    void move_pawn(Square from, Square to, char ch);
    void remove_piece(Square square);
    void move_piece(Square from, Square to);
    void move_king(Square from, Square to);
    void move_rook(Square from, Square to);
};

#endif  // INCLUDE_BOARD_H_
