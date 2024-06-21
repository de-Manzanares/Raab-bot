#include "../include/Board.h"

using s = Square;
using c = Color;

std::vector<std::vector<Square>> Diagonals::leftRight = {
        {s::a6, s::b7, s::c8},
        {s::a5, s::b6, s::c7, s::d8},
        {s::a4, s::b5, s::c6, s::d7, s::e8},
        {s::a3, s::b4, s::c5, s::d6, s::e7, s::f8},
        {s::a2, s::b3, s::c4, s::d5, s::e6, s::f7, s::g8},
        {s::a1, s::b2, s::c3, s::d4, s::e5, s::f6, s::g7, s::h8},
        {s::b1, s::c2, s::d3, s::e4, s::f5, s::g6, s::h7},
        {s::c1, s::d2, s::e3, s::f4, s::g5, s::h6},
        {s::d1, s::e2, s::f3, s::g4, s::h5},
        {s::e1, s::f2, s::g3, s::h4},
        {s::f1, s::g2, s::h3}};
std::vector<std::vector<Square>> Diagonals::rightLeft = {
        {s::h6, s::g7, s::f8},
        {s::h5, s::g6, s::f7, s::e8},
        {s::h4, s::g5, s::f6, s::e7, s::d8},
        {s::h3, s::g4, s::f5, s::e6, s::d7, s::c8},
        {s::h2, s::g3, s::f4, s::e5, s::d6, s::c7, s::b8},
        {s::h1, s::g2, s::f3, s::e4, s::d5, s::c6, s::b7, s::a8},
        {s::g1, s::f2, s::e3, s::d4, s::c5, s::b6, s::a7},
        {s::f1, s::e2, s::d3, s::c4, s::b5, s::a6},
        {s::e1, s::d2, s::c3, s::b4, s::a5},
        {s::d1, s::c2, s::b3, s::a4},
        {s::c1, s::b2, s::a3}};

//----------------------------------------------------------------------------------------------------------------------
// BEGIN Boundary detection

bool is_upper_vertical_boundary(Square sq) { return static_cast<int>(sq) > 55; }

bool is_lower_vertical_boundary(Square sq) { return static_cast<int>(sq) < 8; }

bool is_left_horizontal_boundary(Square sq)
{
    return sq == s::a1 || sq == s::a2 || sq == s::a3 || sq == s::a4
            || sq == s::a5 || sq == s::a6 || sq == s::a7 || sq == s::a8;
}

bool is_right_horizontal_boundary(Square sq)
{
    return sq == s::h1 || sq == s::h2 || sq == s::h3 || sq == s::h4
            || sq == s::h5 || sq == s::h6 || sq == s::h7 || sq == s::h8;
}

// END Boundary detection
//----------------------------------------------------------------------------------------------------------------------

/**
 * @brief Clear all the maps in a Maps object.
 */
void Maps::clear()
{
    white_influence.clear();
    black_influence.clear();
    white_pinned.clear();
    black_pinned.clear();
    white_moves.clear();
    black_moves.clear();
}

/**
 * @brief Copy assignment operator
 * @param rhs The Board object to be copied
 * @return A reference to the lhs object after assignment
 */
Board& Board::operator=(const Board& rhs)
{
    if (this != &rhs) {
        b_pawn = rhs.b_pawn;
        b_night = rhs.b_night;
        b_bishop = rhs.b_bishop;
        b_rook = rhs.b_rook;
        b_queen = rhs.b_queen;
        b_king = rhs.b_king;
        w_Pawn = rhs.w_Pawn;
        w_Night = rhs.w_Night;
        w_Bishop = rhs.w_Bishop;
        w_Rook = rhs.w_Rook;
        w_Queen = rhs.w_Queen;
        w_King = rhs.w_King;
        game_state = rhs.game_state;
    }
    return *this;
}

Board::~Board() { delete maps; }

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

/**
 * @brief Removes a piece from the chessboard at the given square.
 * @details This function updates the bitboards for each piece type by clearing the bit corresponding to the given
 * square. The piece_map is then updated to reflect the new bitmaps.
 * @param square The square to remove the piece from
 */
void Board::remove_piece(Square square)
{
    uint64_t mask = ~(1ULL << static_cast<uint>(square));
    for (auto& [ch, bits] : piece_map) {
        bits &= mask;
    }
}

/**
 * @brief Place a piece on the chessboard
 * @details This function places the specified piece on the given square of the chessboard.
 * The piece is identified by a character, and the square is specified by a value from the Square enum.
 * @param square The square on the chessboard where the piece should be placed
 * @param ch The character representing the piece to be placed
 * @warning assumes the given square is empty
 */
void Board::place_piece(Square square, const char& ch)
{
    for (auto& piece : piece_map) {
        if (ch == piece.first) {
            piece.second |= 1ULL << static_cast<uint>(square);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
// BEGIN piece detection

/**
 * @brief Retrieves the piece on the specified square
 * @param sq The square on the chessboard
 * @return The character representing the piece on the square, or ' ' if the square is empty
 */
char Board::what_piece(Square sq) const
{
    for (const auto& [key, value] : piece_map) {
        if (value & (1ULL << static_cast<int>(sq))) { return key; }
    }
    return ' ';
}

/**
 * @brief Check if a square contains a white rook
 * @param sq The square to check
 * @return True if the square contains a white rook, false otherwise
 */
bool Board::is_white_rook(Square sq) const { return w_Rook & (1ULL << static_cast<int>(sq)); }

/**
 * @brief Checks if a square contains a black rook.
 * @param sq The square to check.
 * @return True if the square contains a black rook, false otherwise.
 */
bool Board::is_black_rook(Square sq) const { return b_rook & (1ULL << static_cast<int>(sq)); }

/**
 * @brief Check if the given square is a rook
 * @param sq The square to check
 * @return True if the square contains a rook, False otherwise
 */
bool Board::is_rook(Square sq) const { return is_white_rook(sq) || is_black_rook(sq); }

/**
 * @brief Check if the given square is a white bishop
 * @param sq The square to check
 * @return True if the square contains a white bishop, false otherwise
 */
bool Board::is_white_bishop(Square sq) const { return w_Bishop & (1ULL << static_cast<int>(sq)); }

/**
 * @brief Check if a given square contains a black bishop
 * @param sq The square to check
 * @return True if the square contains a black bishop, false otherwise
 */
bool Board::is_black_bishop(Square sq) const { return b_bishop & (1ULL << static_cast<int>(sq)); }

/**
 * @brief Check if the given square is a bishop
 * @param sq The square to check
 * @return True if the square contains a bishop, false otherwise
 */
bool Board::is_bishop(Square sq) const { return is_white_bishop(sq) || is_black_bishop(sq); }

/**
 * @brief Check if the given square is a white queen
 * @param sq The square to check
 * @return True if the square contains a white queen, false otherwise
 */
bool Board::is_white_queen(Square sq) const { return w_Queen & (1ULL << static_cast<int>(sq)); }

/**
 * @brief Check if the given square is a black queen
 * @param sq The square to check
 * @return True if the square contains a black queen, false otherwise
 */
bool Board::is_black_queen(Square sq) const { return b_queen & (1ULL << static_cast<int>(sq)); }

/**
 * @brief Check if the given square is a queen
 * @param sq The square to check
 * @return True if the square contains a queen, false otherwise
 */
bool Board::is_queen(Square sq) const { return is_white_queen(sq) || is_black_queen(sq); }

/**
 * @brief Check if the given square is a white knight
 * @param sq The square to check
 * @return True if the square contains a white knight, false otherwise
 */
bool Board::is_white_knight(Square sq) const { return w_Night & (1ULL << static_cast<int>(sq)); }

/**
 * @brief Check if the given square is a black knight
 * @param sq The square to check
 * @return True if the square contains a black knight, false otherwise
 */
bool Board::is_black_knight(Square sq) const { return b_night & (1ULL << static_cast<int>(sq)); }

/**
 * @brief Check if the given square is a knight
 * @param sq The square to check
 * @return True if the square contains a knight, false otherwise
 */
bool Board::is_knight(Square sq) const { return is_white_knight(sq) || is_black_knight(sq); }

/**
 * @brief Check if the given square is a white king
 * @param sq The square to check
 * @return True if the square contains a white king, false otherwise
 */
bool Board::is_white_king(Square sq) const { return w_King & (1ULL << static_cast<int>(sq)); }

/**
 * @brief Check if the given square is a black king
 * @param sq The square to check
 * @return True if the square contains a black king, false otherwise
 */
bool Board::is_black_king(Square sq) const { return b_king & (1ULL << static_cast<int>(sq)); }

/**
 * @brief Check if the given square is a king
 * @param sq The square to check
 * @return True if the square contains a king, false otherwise
 */
bool Board::is_king(Square sq) const { return is_white_king(sq) || is_black_king(sq); }

/**
 * @brief Check if the given square is a white pawn
 * @param sq The square to check
 * @return True if the square contains a white pawn, false otherwise
 */
bool Board::is_white_pawn(Square sq) const { return w_Pawn & (1ULL << static_cast<int>(sq)); }

/**
 * @brief Check if the given square is a black pawn
 * @param sq The square to check
 * @return True if the square contains a black pawn, false otherwise
 */
bool Board::is_black_pawn(Square sq) const { return b_pawn & (1ULL << static_cast<int>(sq)); }

/**
 * @brief Check if the given square is a pawn
 * @param sq The square to check
 * @return True if the square contains a pawn, false otherwise
 */
bool Board::is_pawn(Square sq) const { return is_white_pawn(sq) || is_black_pawn(sq); }

/**
 * @brief Check if the given square is a white piece
 * @param sq The square to check
 * @return True if the square contains a white piece, false otherwise
 */
bool Board::is_white(Square sq) const
{
    return is_white_pawn(sq) || is_white_king(sq) || is_white_knight(sq)
            || is_white_bishop(sq) || is_white_rook(sq) || is_white_queen(sq);
}

/**
 * @brief Check if the given square is a black piece
 * @param sq The square to check
 * @return True if the square contains a black piece, false otherwise
 */
bool Board::is_black(Square sq) const
{
    return is_black_pawn(sq) || is_black_king(sq) || is_black_knight(sq)
            || is_black_bishop(sq) || is_black_rook(sq) || is_black_queen(sq);
}

/**
 * @brief Check the color of a piece on the given square
 * @param sq The square to check
 * @return The color of the piece on the square (white, black, or none)
 */
Color Board::what_color(Square sq) const
{
    Color color;
    if (is_white(sq)) { color = Color::white; }
    else if (is_black(sq)) { color = Color::black; }
    else { color = Color::none; }
    return color;
}

/**
 * @brief Check if a square contains a piece of the same color as the specified color
 * @param sq The square to check
 * @param color The color to compare with
 * @return True if the square has the same color as the specified color, false otherwise
 */
bool Board::is_same_color(Square sq, Color color) const { return what_color(sq) == color; }

/**
 * @brief Check if the given square is empty
 * @param sq The square to check
 * @return True if the square is empty, false otherwise
 */
bool Board::is_empty(Square sq) const { return what_piece(sq) == ' '; }

/**
 * @brief Checks if the enemy king is on a given square.
 * @param sq The square to check.
 * @param c Our color
 * @return True if the enemy king is present on the square, false otherwise.
 */
bool Board::is_opposite_king(Square sq, Color c) const
{
    return c == Color::white ? is_black_king(sq) : is_white_king(sq);
}

/**
 * @brief Get the row of a given square
 * @param sq The square to check
 * @return The number of the row
 */
int Board::get_row(Square sq)
{
    if (sq >= s::h1 && sq <= s::a1) { return 1; }
    if (sq >= s::h2 && sq <= s::a2) { return 2; }
    if (sq >= s::h3 && sq <= s::a3) { return 3; }
    if (sq >= s::h4 && sq <= s::a4) { return 4; }
    if (sq >= s::h5 && sq <= s::a5) { return 5; }
    if (sq >= s::h6 && sq <= s::a6) { return 6; }
    if (sq >= s::h7 && sq <= s::a7) { return 7; }
    if (sq >= s::h8 && sq <= s::a8) { return 8; }
    return 0;
}

/**
 * @brief Get the column of a given square
 * @param sq The square to check
 * @return The column number. (a:h) = (1:8)
 */
int Board::get_column(Square sq)
{
    if (static_cast<int>(sq) % 8 == 0) { return 8; }
    if (static_cast<int>(sq) % 8 == 1) { return 7; }
    if (static_cast<int>(sq) % 8 == 2) { return 6; }
    if (static_cast<int>(sq) % 8 == 3) { return 5; }
    if (static_cast<int>(sq) % 8 == 4) { return 4; }
    if (static_cast<int>(sq) % 8 == 5) { return 3; }
    if (static_cast<int>(sq) % 8 == 6) { return 2; }
    if (static_cast<int>(sq) % 8 == 7) { return 1; }
    return 0;
}

/**
 * @brief Check if two squares are in the same row.
 * @param sq1 The first square.
 * @param sq2 The second square.
 * @return True if the squares are in the same row, false otherwise.
 */
bool Board::is_inSameRow(Square sq1, Square sq2) { return get_row(sq1) == get_row(sq2); }

/**
 * @brief Check if three squares are in the same row
 * @param sq1 First square
 * @param sq2 Second square
 * @param sq3 Third square
 * @return True if all three squares are in the same row, false otherwise
 */
bool Board::is_inSameRow(Square sq1, Square sq2, Square sq3)
{
    return is_inSameRow(sq1, sq2) && is_inSameRow(sq2, sq3);
}

/**
 * @brief Check if two squares are in the same column
 * @param sq1 The first square
 * @param sq2 The second square
 * @return True if the squares are in the same column, false otherwise
 */
bool Board::is_inSameColumn(Square sq1, Square sq2) { return get_column(sq1) == get_column(sq2); }

/**
 * Check if three squares are in the same column.
 * @param sq1 First square.
 * @param sq2 Second square.
 * @param sq3 Third square.
 * @return True if all three squares are in the same column, false otherwise.
 */
bool Board::is_inSameColumn(Square sq1, Square sq2, Square sq3)
{
    return is_inSameColumn(sq1, sq2) && is_inSameColumn(sq2, sq3);
}

/**
 * @brief Check if two squares are on the same diagonal in Diagonals::leftRight
 * @param sq1 The first square
 * @param sq2 The second square
 * @return True if the squares are on the same diagonal false otherwise
 */
bool Board::is_inSameDiagonal_leftRight(Square sq1, Square sq2)
{
    for (const auto& v : Diagonals::leftRight) {
        for (const auto& e : v) { if (sq1 == e) { for (const auto& e2 : v) { if (sq2 == e2) { return true; }}}}
    }
    return false;
}

/**
 * @brief Check if three squares are in the same diagonal in Diagonals::leftRight
 * @param sq1 The first square.
 * @param sq2 The second square.
 * @param sq3 The third square.
 * @return True if the three squares are in the same diagonal, false otherwise.
 */
bool Board::is_inSameDiagonal_leftRight(Square sq1, Square sq2, Square sq3)
{
    return is_inSameDiagonal_leftRight(sq1, sq2) && is_inSameDiagonal_leftRight(sq2, sq3);
}

/**
 * @brief Check if two squares are in the same diagonal in Diagonals::rightLeft
 * @param sq1 The first square
 * @param sq2 The second square
 * @return True if the squares are in the same diagonal, false otherwise
 */
bool Board::is_inSameDiagonal_rightLeft(Square sq1, Square sq2)
{
    for (const auto& v : Diagonals::rightLeft) {
        for (const auto& e : v) {
            if (sq1 == e) {
                for (const auto& e2 : v) {
                    if (sq2 == e2) { return true; }
                }
            }
        }
    }
    return false;
}

/**
 * @brief Check if three squares are in the same diagonal in Diagonals::rightLeft
 * @param sq1 The first square
 * @param sq2 The second square
 * @param sq3 The third square
 * @return true if the three squares are in the same diagonal, false otherwise
 */
bool Board::is_inSameDiagonal_rightLeft(Square sq1, Square sq2, Square sq3)
{
    return is_inSameDiagonal_rightLeft(sq1, sq2) && is_inSameDiagonal_rightLeft(sq2, sq3);
}

// END piece detection
//----------------------------------------------------------------------------------------------------------------------
// BEGIN FEN

/**
 * @brief Check if a specific bit is set in a given unsigned 64-bit integer.
 * @details A set bit means the a piece is on the board in that square, 0 bit means 0 piece
 * @param n The unsigned 64-bit integer to check.
 * @param sq The square (index of the uint64_t) to check.
 * @return True if the bit at the specified square is set, false otherwise.
 */
bool isBitSet(uint64_t n, Square sq) { return n & (1ULL << static_cast<int>(sq)); }

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
 * @brief Export the current state of the board as FEN (Forsyth–Edwards Notation) string.
 * @details
 * 1. piece placement \n
 * 2. active color \n
 * 3. castling ability '-' if neither side has the ability \n
 * 4. en passant target squares '-' if there are no target squares \n
 * 5. half move clock, for fifty move rule
 * 6. full move number
 * @return std::string Returns the FEN string representing the board state.
 */
std::string Board::export_fen() const
{
    std::string fen;    // string to be exported
    fen += fen_piece_placement();
    fen += ' ';
    fen += game_state.fen_active_color();
    fen += ' ';
    fen += game_state.fen_castling_ability();
    fen += ' ';
    fen += game_state.fen_en_passant_targets();
    fen += ' ';
    fen += game_state.fen_half_move_clock();
    fen += ' ';
    fen += game_state.fen_full_move_number();
    return fen;
}

/**
 * @brief Helper function for Board::set_pieces
 * @details Checks if a character is a valid chess piece.
 * @param ch The character to be checked.
 * @return True if the character is a valid chess piece, otherwise False.
 */
bool is_piece(const char& ch)
{
    return ch == 'p' || ch == 'n' || ch == 'b' || ch == 'r' || ch == 'q' || ch == 'k' ||
            ch == 'P' || ch == 'N' || ch == 'B' || ch == 'R' || ch == 'Q' || ch == 'K';
}

/**
 * Sets the pieces on the chessboard based on the given FEN string
 * @param fen The FEN string representing the piece placement
 * @return The number of characters processed in the FEN string
 */
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

/**
 * Sets the board from a given FEN string.
 * @param fen    The FEN string to import.
 */
void Board::import_fen(const std::string& fen)
{
    std::string game_state_string;
    std::string temp;

    // clear the board
    clear();

    // set pieces and create game state string
    game_state_string = fen.substr(set_pieces(fen));

    // set games state variables
    std::istringstream iss(game_state_string);

    // create variables
    char active_color;
    std::string castling_ability;
    std::string en_passant_target;
    uint half_move_clock;
    uint full_move_number;

    // gather data
    iss >> active_color >> castling_ability >> en_passant_target >> half_move_clock >> full_move_number;

    // assign to board variables
    // active color
    active_color == 'w' ? game_state.active_color = Color::white : game_state.active_color = Color::black;

    // castling ability
    game_state.set_castling_ability(castling_ability);
    game_state.en_passant_target = en_passant_target;
    game_state.half_move_clock = half_move_clock;
    game_state.full_move_number = full_move_number;
}

// END FEN
//----------------------------------------------------------------------------------------------------------------------
// BEGIN legal moves

/**
 * @brief Calculates the legal moves for a pawn on the board
 * @param sq The square on which the pawn is located
 * @return A vector of squares representing the legal moves for the pawn
 */
std::vector<Square> Board::legal_moves_pawn(Square sq) const
{
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

/**
 * @brief Get the legal moves for the piece in a given square on the chessboard
 * @param sq The square for which to calculate the legal moves
 * @return A vector of Squares representing the legal moves
 */
std::vector<Square> Board::legal_moves(Square sq)
{
    std::vector<Square> moves{};
    std::vector<Square> temp = influence(sq);

    if (!is_pawn(sq)) {
        for (const auto& square : temp) {
            if (!is_same_color(sq, what_color(square))) { moves.push_back(square); }
        }
    }
    if (is_pawn(sq)) {
        Square ept{};
        bool en_passant = false;
        if (!game_state.en_passant_target.empty()) {
            ept = Sq::string_to_square(game_state.en_passant_target);
            en_passant = true;
        }
        for (const auto& square : temp) {
            if (!is_same_color(sq, what_color(square)) && !is_empty(square)) { moves.push_back(square); }
            if (en_passant && square == ept) { if (get_row(ept) == 3 && is_black_pawn(sq)) { moves.push_back(ept); }}
            if (en_passant && square == ept) { if (get_row(ept) == 6 && is_white_pawn(sq)) { moves.push_back(ept); }}
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
// BEGIN influence rook

/**
 * @brief Computes the influence of a rook on the board, given a specific square.
 * @param sq The square to compute the influence for.
 * @return std::vector<Square> A vector of squares representing the influence of the rook.
 */
std::vector<Square> Board::influence_rook(Square sq) const
{
    std::vector<Square> influence{};
    influence.reserve(16);
    // vertical up
    for (auto square = sq + 8; !is_upper_vertical_boundary(square - 8); square = square + 8) {
        influence.push_back(square);
        if (!is_empty(square) && !is_opposite_king(square, what_color(sq))) { break; }
    }
    // vertical down
    for (auto square = sq - 8; !is_lower_vertical_boundary(square + 8); square = square - 8) {
        influence.push_back(square);
        if (!is_empty(square) && !is_opposite_king(square, what_color(sq))) { break; }
    }
    // horizontal right
    for (auto square = sq - 1; !is_right_horizontal_boundary(square + 1); square = square - 1) {
        influence.push_back(square);
        if (!is_empty(square) && !is_opposite_king(square, what_color(sq))) { break; }
    }
    // horizontal left
    for (auto square = sq + 1; !is_left_horizontal_boundary(square - 1); square = square + 1) {
        influence.push_back(square);
        if (!is_empty(square) && !is_opposite_king(square, what_color(sq))) { break; }
    }
    return influence;
}

// END influence rook
//----------------------------------------------------------------------------------------------------------------------
// BEGIN influence bishop

/**
 * @brief Calculate the squares influenced by a bishop piece on the chessboard.
 * @param sq The square where the bishop is located.
 * @return std::vector<Square> A vector containing all the squares influenced by the bishop.
 */
std::vector<Square> Board::influence_bishop(Square sq) const
{
    std::vector<Square> influence{};
    influence.reserve(16);
    // up left
    for (auto square = sq + 9;
         !is_upper_vertical_boundary(square - 9) && !is_left_horizontal_boundary(square - 9);
         square = square + 9) {
        influence.push_back(square);
        if (!is_empty(square) && !is_opposite_king(square, what_color(sq))) { break; }
    }
    // up right
    for (auto square = sq + 7;
         !is_upper_vertical_boundary(square - 7) && !is_right_horizontal_boundary(square - 7);
         square = square + 7) {
        influence.push_back(square);
        if (!is_empty(square) && !is_opposite_king(square, what_color(sq))) { break; }
    }
    // down right
    for (auto square = sq - 9;
         !is_lower_vertical_boundary(square + 9) && !is_right_horizontal_boundary(square + 9);
         square = square - 9) {
        influence.push_back(square);
        if (!is_empty(square) && !is_opposite_king(square, what_color(sq))) { break; }
    }
    // down left
    for (auto square = sq - 7;
         !is_lower_vertical_boundary(square + 7) && !is_left_horizontal_boundary(square + 7);
         square = square - 7) {
        influence.push_back(square);
        if (!is_empty(square) && !is_opposite_king(square, what_color(sq))) { break; }
    }
    return influence;
}

// END influence bishop
//----------------------------------------------------------------------------------------------------------------------
// BEGIN influence queen

/**
 * @brief Calculate the squares influenced by a queen on the chessboard
 * @param sq The square where the queen is located
 * @return std::vector<Square> The squares influenced by the queen
 */
std::vector<Square> Board::influence_queen(Square sq) const
{
    std::vector<Square> v1 = influence_rook(sq);
    std::vector<Square> v2 = influence_bishop(sq);
    std::vector<Square> influence;
    influence.reserve(32);
    influence.insert(influence.end(), v1.begin(), v1.end());
    influence.insert(influence.end(), v2.begin(), v2.end());
    return influence;
}

// END influence queen
//----------------------------------------------------------------------------------------------------------------------
// BEGIN influence knight

/**
 * @brief Calculates the squares influenced by a knight on a given square.
 * @param sq The square where the knight is located.
 * @return A vector of squares influenced by the knight.
 */
std::vector<Square> Board::influence_knight(Square sq)
{
    std::vector<Square> influence{};
    influence.reserve(8);
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

// END influence knight
//----------------------------------------------------------------------------------------------------------------------
// BEGIN influence king

/**
 * @brief Calculates the squares that are influenced by the king at a given square.
 * @param sq The square where the king is located.
 * @return A vector of squares that are influenced by the king.
 */
std::vector<Square> Board::influence_king(Square sq)
{
    std::vector<Square> influence{};
    influence.reserve(16);
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

// END influence king
//----------------------------------------------------------------------------------------------------------------------
// BEGIN influence pawn

/**
 * @brief Calculates the squares influenced by a pawn on the chessboard
 * @param sq The square where the pawn is located
 * @return A vector of squares influenced by the pawn
 */
std::vector<Square> Board::influence_pawn(Square sq) const
{
    std::vector<Square> influence{};
    influence.reserve(8);
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

// END influence pawn
//----------------------------------------------------------------------------------------------------------------------
// BEGIN influence

/**
 * @brief Creates a list of squares that a given piece is currently "influencing".
 * @param sq A given square.
 * @return A list of squares that the piece on the given square is currently "influencing".
 */
std::vector<Square> Board::influence(Square sq) const
{
    std::vector<Square> influence{};
    influence.reserve(32);

    // if the square is empty, there is no influence
    if (what_piece(sq) == ' ') { return influence; }
    else if (is_pawn(sq)) { influence = influence_pawn(sq); }
    else if (is_rook(sq)) { influence = influence_rook(sq); }
    else if (is_bishop(sq)) { influence = influence_bishop(sq); }
    else if (is_queen(sq)) { influence = influence_queen(sq); }
    else if (is_knight(sq)) { influence = influence_knight(sq); }
    else if (is_king(sq)) { influence = influence_king(sq); }
    return influence;
}

// END influence
//----------------------------------------------------------------------------------------------------------------------
// BEGIN update influence maps

/**
 * @brief Update the influence maps for white and black pieces
 * This function resets the influence_map_white and influence_map_black maps, and then calculates the influence of each
 * white and black piece on the board. The influence is the set of squares that a piece can attack or control.
 */
void Board::update_influence_maps() const
{
    // reset the maps
    maps->white_influence.clear();
    maps->black_influence.clear();

    for (Square sq = Square::a8; sq >= Square::h1; --sq) {
        // white moves
        if (is_white(sq)) {
            maps->white_influence.insert({sq, influence(sq)});
        }
            // black moves
        else if (is_black(sq)) {
            maps->black_influence.insert({sq, influence(sq)});
        }
    }
}

// END update influence maps
//----------------------------------------------------------------------------------------------------------------------
// BEGIN pinned piece rook

/**
 * @brief Finds the piece pinned by a given rook
 * @details This function searches in the vertical and horizontal directions from the given square to find
 * any potentially pinned pieces
 * @param sq The square of the rook.
 * @return The square of the pinned piece, or the square of the given rook if there is no pinned piece
 */
Square Board::pinned_piece_rook(Square sq) const
{
    Square pinned;                  // potentially pinned piece
    bool found_one;                 // xray through ONE enemy piece
    Color c = what_color(sq);

    // vertical up
    found_one = false;
    for (auto square = sq + 8; !is_upper_vertical_boundary(square - 8); square = square + 8) {
        if (!is_empty(square) && is_same_color(square, c)) { break; }
        else if (found_one) {
            // after the first non-king opponent has been found, if we find another, end searching in this direction
            if (!is_empty(square) && !is_same_color(square, c) && !is_opposite_king(square, c)) { break; }
            // after the first ... if we find the opposing king, that first piece is pinned.  YAY
            if (is_opposite_king(square, c)) {
                return pinned;
            }
        }
            // if it is occupied by an opponent that is not a king
        else if (!is_empty(square) && !is_same_color(square, c) && !is_opposite_king(square, c)) {
            pinned = square;    // possible pinned piece
            found_one = true;
        }
    }
    // vertical down
    found_one = false;
    for (auto square = sq - 8; !is_lower_vertical_boundary(square + 8); square = square - 8) {
        if (!is_empty(square) && is_same_color(square, c)) { break; }
        else if (found_one) {
            // after the first non-king opponent has been found, if we find another, end searching in this direction
            if (!is_empty(square) && !is_same_color(square, c) && !is_opposite_king(square, c)) { break; }
            // after the first ... if we find the opposing king, that first piece is pinned.  YAY
            if (is_opposite_king(square, c)) {
                return pinned;
            }
        }
            // if it is occupied by an opponent that is not a king
        else if (!is_empty(square) && !is_same_color(square, c) && !is_opposite_king(square, c)) {
            pinned = square;    // possible pinned piece
            found_one = true;
        }
    }
    // horizontal right
    found_one = false;
    for (auto square = sq - 1; !is_right_horizontal_boundary(square + 1); square = square - 1) {
        if (!is_empty(square) && is_same_color(square, c)) { break; }
        else if (found_one) {
            // after the first non-king opponent has been found, if we find another, end searching in this direction
            if (!is_empty(square) && !is_same_color(square, c) && !is_opposite_king(square, c)) { break; }
            // after the first ... if we find the opposing king, that first piece is pinned.  YAY
            if (is_opposite_king(square, c)) {
                return pinned;
            }
        }
            // if it is occupied by an opponent that is not a king
        else if (!is_empty(square) && !is_same_color(square, c) && !is_opposite_king(square, c)) {
            pinned = square;    // possible pinned piece
            found_one = true;
        }
    }
    // horizontal left
    found_one = false;
    for (auto square = sq + 1; !is_left_horizontal_boundary(square - 1); square = square + 1) {
        if (!is_empty(square) && is_same_color(square, c)) { break; }
        else if (found_one) {
            // after the first non-king opponent has been found, if we find another, end searching in this direction
            if (!is_empty(square) && !is_same_color(square, c) && !is_opposite_king(square, c)) { break; }
            // after the first ... if we find the opposing king, that first piece is pinned.  YAY
            if (is_opposite_king(square, c)) {
                return pinned;
            }
        }
            // if it is occupied by an opponent that is not a king
        else if (!is_empty(square) && !is_same_color(square, c) && !is_opposite_king(square, c)) {
            pinned = square;    // possible pinned piece
            found_one = true;
        }
    }
    return sq;
}

// END pinned pieces rook
//----------------------------------------------------------------------------------------------------------------------
// BEGIN pinned piece bishop

/**
 * @brief Find the potentially pinned piece on the board from the given bishop
 * @details This function searches the diagonal directions outward from a given bishop to see if the bishop is pinning
 * any enemy pieces.
 * @param sq The square to start the search from.
 * @return The square of the pinned piece, or the square of the bishop if no piece is pinned.
 */
Square Board::pinned_piece_bishop(Square sq) const
{
    Square pinned;                  // potentially pinned piece
    bool found_one;                 // xray through ONE enemy piece
    Color c = what_color(sq);

    // up left
    found_one = false;
    for (auto square = sq + 9;
         !is_upper_vertical_boundary(square - 9) && !is_left_horizontal_boundary(square - 9);
         square = square + 9) {
        if (!is_empty(square) && is_same_color(square, c)) { break; }
        else if (found_one) {
            // after the first non-king opponent has been found, if we find another, end searching in this direction
            if (!is_empty(square) && !is_same_color(square, c) && !is_opposite_king(square, c)) { break; }
            // after the first ... if we find the opposing king, that first piece is pinned.  YAY
            if (is_opposite_king(square, c)) {
                return pinned;
            }
        }
            // if it is occupied by an opponent that is not a king
        else if (!is_empty(square) && !is_same_color(square, c) && !is_opposite_king(square, c)) {
            pinned = square;    // possible pinned piece
            found_one = true;
        }
    }
    // up right
    found_one = false;
    for (auto square = sq + 7;
         !is_upper_vertical_boundary(square - 7) && !is_right_horizontal_boundary(square - 7);
         square = square + 7) {
        if (!is_empty(square) && is_same_color(square, c)) { break; }
        else if (found_one) {
            // after the first non-king opponent has been found, if we find another, end searching in this direction
            if (!is_empty(square) && !is_same_color(square, c) && !is_opposite_king(square, c)) { break; }
            // after the first ... if we find the opposing king, that first piece is pinned.  YAY
            if (is_opposite_king(square, c)) {
                return pinned;
            }
        }
            // if it is occupied by an opponent that is not a king
        else if (!is_empty(square) && !is_same_color(square, c) && !is_opposite_king(square, c)) {
            pinned = square;    // possible pinned piece
            found_one = true;
        }
    }
    // down right
    found_one = false;
    for (auto square = sq - 9;
         !is_lower_vertical_boundary(square + 9) && !is_right_horizontal_boundary(square + 9);
         square = square - 9) {
        if (!is_empty(square) && is_same_color(square, c)) { break; }
        else if (found_one) {
            // after the first non-king opponent has been found, if we find another, end searching in this direction
            if (!is_empty(square) && !is_same_color(square, c) && !is_opposite_king(square, c)) { break; }
            // after the first ... if we find the opposing king, that first piece is pinned.  YAY
            if (is_opposite_king(square, c)) {
                return pinned;
            }
        }
            // if it is occupied by an opponent that is not a king
        else if (!is_empty(square) && !is_same_color(square, c) && !is_opposite_king(square, c)) {
            pinned = square;    // possible pinned piece
            found_one = true;
        }
    }
    // down left
    found_one = false;
    for (auto square = sq - 7;
         !is_lower_vertical_boundary(square + 7) && !is_left_horizontal_boundary(square + 7);
         square = square - 7) {
        if (!is_empty(square) && is_same_color(square, c)) { break; }
        else if (found_one) {
            // after the first non-king opponent has been found, if we find another, end searching in this direction
            if (!is_empty(square) && !is_same_color(square, c) && !is_opposite_king(square, c)) { break; }
            // after the first ... if we find the opposing king, that first piece is pinned.  YAY
            if (is_opposite_king(square, c)) {
                return pinned;
            }
        }
            // if it is occupied by an opponent that is not a king
        else if (!is_empty(square) && !is_same_color(square, c) && !is_opposite_king(square, c)) {
            pinned = square;    // possible pinned piece
            found_one = true;
        }
    }
    return sq;
}

// END pinned piece bishop
//----------------------------------------------------------------------------------------------------------------------
// BEGIN pinned piece queen

/**
 * @brief Find the potentially pinned piece on the board from the given queen
 * @details A queen moves like a rook and bishop combined, don't it?
 * @param sq The square to start the search from.
 * @return The square of the pinned piece, or the square of the queen if no piece is pinned.
 * @see Board::pinned_piece_rook and Board::pinned_piece_bishop
 */
Square Board::pinned_piece_queen(Square sq) const
{
    if (pinned_piece_rook(sq) != sq) { return pinned_piece_rook(sq); }
    if (pinned_piece_bishop(sq) != sq) { return pinned_piece_bishop(sq); }
    return sq;
}

// END pinned piece queen
//----------------------------------------------------------------------------------------------------------------------
// BEGIN pinned pieces

/**
 * @brief Detects if a piece at the given square is pinning an opposing piece to the enemy king
 * @param sq The square to check
 * @return The square of the pinned piece, or the square of the piece we are checking if there is no pin.
 */
Square Board::pinned_piece(Square sq) const
{
    if (is_rook(sq)) { if (pinned_piece_rook(sq) != sq) { return pinned_piece_rook(sq); }}
    if (is_bishop(sq)) { if (pinned_piece_bishop(sq) != sq) { return pinned_piece_bishop(sq); }}
    if (is_queen(sq)) { if (pinned_piece_queen(sq) != sq) { return pinned_piece_queen(sq); }}

    return sq;
}

// END pinned pieces
//----------------------------------------------------------------------------------------------------------------------
// BEGIN update pinned pieces

/**
 * @brief Update the maps of pinned pieces on the chessboard
 * @details Checks every piece to find pins to the enemy king and records pinned pieces
 * @note mutates Maps::black_pinned and Maps::white_pinned
 */
void Board::update_pinned_pieces() const
{
    // reset the maps
    maps->white_pinned.clear();
    maps->black_pinned.clear();

    for (const auto& [sq, moves] : maps->white_influence) {
        if (pinned_piece(sq) != sq) { maps->black_pinned.insert({pinned_piece(sq), sq}); }
    }
    for (const auto& [sq, moves] : maps->black_influence) {
        if (pinned_piece(sq) != sq) { maps->white_pinned.insert({pinned_piece(sq), sq}); }
    }
}

// END update pinned pieces
//----------------------------------------------------------------------------------------------------------------------
// BEGIN update king moves

/**
 * @brief Applies rules of the games to reduce the list of all possible moves for the white king to a list of legal
 * moves for the white king
 * @param sq_w_King The current position of the white king.
 * @return A vector of squares containing pieces that are giving check to the white king.
 * @note May mutate Game_State::white_inCheck
 * @warning For move generation as a whole to function properly, there must be exactly 1 king of each color on the board
 **/
std::vector<Square> Board::update_white_king_moves(Square sq_w_King)
{
    std::vector<Square> giving_check{};
    bool is_in_check;                           // king is in check
    bool under_attack;                          // flags a square as under attack
    std::vector<Square> influence_kings;        // temp vector to create king move list
    std::vector<Square> safe_squares;           // temp vector to create king move list
    std::vector<Square> legal_moves_kings;      // temp vector to create king move list

    // add castle squares if castling is still valid. Must be done before cleaning the list because while castling
    // ability may be present, is may not always be legal to do so, because enemy pieces could be attacking those
    // squares, or friendly pieces may be blocking them.
    // piece giving check

    // get started
    influence_kings = influence(sq_w_King);

    // detect whether king is in check
    is_in_check = false;
    for (const auto& pair : maps->black_influence) {
        if (std::find(pair.second.begin(), pair.second.end(), sq_w_King)
                != pair.second.end()) {  // is the king in check
            is_in_check = true;
            game_state.white_inCheck = true;
            // keep track of who is giving check
            giving_check.push_back(pair.first);
        }
    }
    if (!is_in_check) {     // if not in check, add castling moves (if available)
        if (game_state.castle_w_K) { influence_kings.push_back(s::g1); }
        if (game_state.castle_w_Q) { influence_kings.push_back(s::c1); }
    }

    // remove square that are under attack
    for (const auto& s : influence_kings) {
        under_attack = false;   // reset search flag
        for (const auto& pair : maps->black_influence) {
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
    if (game_state.castle_w_K) {
        if (std::find(legal_moves_kings.begin(), legal_moves_kings.end(), s::g1) != legal_moves_kings.end()) {
            if (std::find(legal_moves_kings.begin(), legal_moves_kings.end(), s::f1) == legal_moves_kings.end()) {
                // remove g1
                auto it = std::remove(legal_moves_kings.begin(), legal_moves_kings.end(), s::g1);
                legal_moves_kings.erase(it, legal_moves_kings.end());
            }
        }
    }
    if (game_state.castle_w_Q) {
        if (std::find(legal_moves_kings.begin(), legal_moves_kings.end(), s::c1) != legal_moves_kings.end()) {
            if (std::find(legal_moves_kings.begin(), legal_moves_kings.end(), s::d1) == legal_moves_kings.end()) {
                // remove c1
                auto it = std::remove(legal_moves_kings.begin(), legal_moves_kings.end(), s::c1);
                legal_moves_kings.erase(it, legal_moves_kings.end());
            }
                // when castling long, gotta check the third square too. if b1 isn't empty, can't castle just yet
            else if (!is_empty(Square::b1)) {
                auto it = std::remove(legal_moves_kings.begin(), legal_moves_kings.end(), s::c1);
                legal_moves_kings.erase(it, legal_moves_kings.end());
            }
        }
    }

    // add king moves to move map
    maps->white_moves.insert({sq_w_King, legal_moves_kings});

    return giving_check;
}

/**
 * @brief Applies rules of the games to reduce the list of all possible moves for the black king to a list of legal
 * moves for the black king
 * @param sq_b_king The current position of the black king.
 * @return A vector of squares containing pieces that are giving check to the white king.
 * @note May mutate Game_State::black_inCheck
 * @warning For move generation as a whole to function properly, there must be exactly 1 king of each color on the board
 **/
std::vector<Square> Board::update_black_king_moves(Square sq_b_king)
{
    std::vector<Square> giving_check{};
    bool is_inCheck;                           // king is in check
    bool under_attack;                          // flags a square as under attack
    std::vector<Square> influence_kings;        // temp vector to create king move list
    std::vector<Square> safe_squares;           // temp vector to create king move list
    std::vector<Square> legal_moves_kings;      // temp vector to create king move list

    // get started
    influence_kings = influence(sq_b_king);

    // detect whether king is in check
    is_inCheck = false;
    for (const auto& pair : maps->white_influence) {
        if (std::find(pair.second.begin(), pair.second.end(), sq_b_king)
                != pair.second.end()) {  // is the king in check
            is_inCheck = true;
            game_state.black_inCheck = true;
            giving_check.push_back(pair.first);
        }
    }
    if (!is_inCheck) {     // if not in check, add castling moves (if available)
        if (game_state.castle_b_k) { influence_kings.push_back(s::g8); }
        if (game_state.castle_b_q) { influence_kings.push_back(s::c8); }
    }

    // remove square that are under attack
    for (const auto& s : influence_kings) {
        under_attack = false;   // reset search flag
        for (const auto& pair : maps->white_influence) {
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
    if (game_state.castle_b_k) {
        if (std::find(legal_moves_kings.begin(), legal_moves_kings.end(), s::g8) != legal_moves_kings.end()) {
            if (std::find(legal_moves_kings.begin(), legal_moves_kings.end(), s::f8) == legal_moves_kings.end()) {
                // remove g8
                auto it = std::remove(legal_moves_kings.begin(), legal_moves_kings.end(), s::g8);
                legal_moves_kings.erase(it, legal_moves_kings.end());
            }
        }
    }
    if (game_state.castle_b_q) {
        if (std::find(legal_moves_kings.begin(), legal_moves_kings.end(), s::c8) != legal_moves_kings.end()) {
            if (std::find(legal_moves_kings.begin(), legal_moves_kings.end(), s::d8) == legal_moves_kings.end()) {
                // remove c1
                auto it = std::remove(legal_moves_kings.begin(), legal_moves_kings.end(), s::c8);
                legal_moves_kings.erase(it, legal_moves_kings.end());
            }
                // when castling long, gotta check the third square too. if b8 isn't empty, can't castle just yet
            else if (!is_empty(Square::b8)) {
                auto it = std::remove(legal_moves_kings.begin(), legal_moves_kings.end(), s::c8);
                legal_moves_kings.erase(it, legal_moves_kings.end());
            }
        }
    }

    // add king moves to move map
    maps->black_moves.insert({sq_b_king, legal_moves_kings});

    return giving_check;
}
// END update king moves
//----------------------------------------------------------------------------------------------------------------------
// BEGIN update move maps

/**
 * @brief Copies influence maps to move maps, excluding kings and pawns
 * @details Move generation is done by starting with the influence maps and reducing the map by applying rules of the
 * game. This function returns the square of (a) king because we will need that info later, and now is as good a time
 * as any to get it.
 * @param c Indicates which map to copy
 * @param from A pointer to the unordered_map from which the squares will be copied
 * @param to A pointer to the unordered_map where the squares will be assigned
 * @return The square in which the king resides
 */
Square Board::copy_influence(const Color c, std::unordered_map<Square, std::vector<Square>> *from,
        std::unordered_map<Square, std::vector<Square>> *to) const
{
    Square sq_king{};
    to->clear();
    if (c == c::white) {
        for (const auto& pair : *from) {
            if (!is_white_pawn(pair.first) && !is_white_king(pair.first)) { to->insert(pair); }
            else if (is_white_king(pair.first)) { sq_king = pair.first; }
        }
    }
    else if (c == c::black) {
        for (const auto& pair : *from) {
            if (!is_black_pawn(pair.first) && !is_black_king(pair.first)) { to->insert(pair); }
            else if (is_black_king(pair.first)) { sq_king = pair.first; }
        }
    }
    return sq_king;
}

/**
 * @brief Extracts legal moves from influence maps
 * @details It is not legal to capture your own pieces
 * @param map A pointer to the unordered_map storing squares and their corresponding vectors of squares
 * @param color The color of the squares to be removed
 */
void Board::remove_same_color_squares(Color color, std::unordered_map<Square, std::vector<Square>> *map) const
{
    std::unordered_map<Square, std::vector<Square>> temp;   // to reassign original after cleaning
    std::vector<Square> temp_squares{};                     // to store non-same-colored squares
    temp_squares.reserve(16);

    for (const auto& pair : *map) {
        temp_squares.clear();                               // reset temp vector
        // record every non-same-colored square
        for (const auto& sq : pair.second) { if (!is_same_color(sq, color)) { temp_squares.push_back(sq); }}
        temp.insert({pair.first, temp_squares});            // insert into "scrubbed" map
    }
    *map = temp;                                            // reassign original map
}

/**
 * @brief Move generation.
 * @details Move maps are created by starting with all possible moves for a piece using update_influence_maps(), then
 * reducing this list by applying various rules of the game
 * @note Mutates `Maps`
 */
void Board::update_move_maps()
{
    // setup ----------------------------------------------
    maps->clear();                          // clear move maps
    game_state.white_inCheck = false;       // reset check flags
    game_state.black_inCheck = false;
    update_influence_maps();                // update influence maps

    // copy influence maps, exclude pawns and kings; we have a special treatment for them
    Square sq_w_King = copy_influence(Color::white, &maps->white_influence, &maps->white_moves);
    Square sq_b_king = copy_influence(Color::black, &maps->black_influence, &maps->black_moves);

    update_pinned_pieces();   // update pinned pieces

    // start reduction ------------------------------------
    remove_same_color_squares(Color::white, &maps->white_moves);    // remove same colored squares
    remove_same_color_squares(Color::black, &maps->black_moves);    // remove same colored squares

    for (const auto& [sq, moves] : maps->white_influence) {
        if (is_white_pawn(sq)) { maps->white_moves.insert({sq, legal_moves(sq)}); }     // generate white pawn moves
    }
    for (const auto& [sq, moves] : maps->black_influence) {
        if (is_black_pawn(sq)) { maps->black_moves.insert({sq, legal_moves(sq)}); }     // generate black pawn moves
    }

    // generate king moves
    // if king is in check, go through and "scrub" the move map
    std::vector<Square> giving_check{};
    std::vector<Square> blocking_squares{};

    // white
    giving_check = update_white_king_moves(sq_w_King);

    // search for pinned pieces
    // the piece can only move along the line of the pin
    // if the king is in the same row or column as a rook or queen
    // if the king is in the same diagonal as a bishop
    // and there is only one friendly piece sharing that line - that piece is pinned
    // the pieces movement must be restricted to the line of the pin
    // there may be multiple lines of pinning

    // find all the rows, columns, and diagonals that the king shares with opposite colored rooks, queens, and bishops

    // if one piece is giving check, it can be captured or blocked
    if (giving_check.size() == 1) {
        // if it's not a knight or pawn, it might be able to be blocked
        if (!is_knight(giving_check[0]) && !is_pawn(giving_check[0])) {
            if (is_inSameRow(giving_check[0], sq_w_King)) {
                for (auto sq = sq_w_King; sq != giving_check[0]; sq_w_King > giving_check[0] ? --sq : ++sq) {
                    blocking_squares.push_back(sq);
                }
            }
            else if (is_inSameColumn(giving_check[0], sq_w_King)) {
                for (auto sq = sq_w_King;
                     sq != giving_check[0];
                     sq_w_King > giving_check[0] ? sq = sq - 8 : sq = sq + 8) {
                    blocking_squares.push_back(sq);
                }
            }
            else if (is_inSameDiagonal_leftRight(giving_check[0], sq_w_King)) {
                for (auto sq = sq_w_King;
                     sq != giving_check[0];
                     sq_w_King > giving_check[0] ? sq = sq - 7 : sq = sq + 7) {
                    blocking_squares.push_back(sq);
                }
            }
            else if (is_inSameDiagonal_rightLeft(giving_check[0], sq_w_King)) {
                for (auto sq = sq_w_King;
                     sq != giving_check[0];
                     sq_w_King > giving_check[0] ? sq = sq - 9 : sq = sq + 9) {
                    blocking_squares.push_back(sq);
                }
            }
        }
        for (auto& pair : maps->white_moves) {
            if (!is_king(pair.first)) {
                std::vector<Square> temp{};
                for (const auto& sq : pair.second) {
                    for (const auto& block_sq : blocking_squares) {
                        if (sq == block_sq) { temp.push_back(sq); }
                    }
                    if (sq == giving_check[0]) { temp.push_back(sq); }
                }
                pair.second = temp;
            }
        }
    }
        // if more than once piece is giving check, the king must be moved
    else if (giving_check.size() > 1) {
        // clear all moves except king evasion
        for (auto& pair : maps->white_moves) {
            if (!is_king(pair.first)) { pair.second.clear(); }
        }
    }

    // black
    giving_check.clear();
    giving_check = update_black_king_moves(sq_b_king);
    blocking_squares.clear();   // reset
    // if one piece is giving check, it can be captured or blocked
    if (giving_check.size() == 1) {
        if (!is_knight(giving_check[0]) && !is_pawn(giving_check[0])) {
            if (is_inSameRow(giving_check[0], sq_b_king)) {
                for (auto sq = sq_b_king; sq != giving_check[0]; sq_b_king > giving_check[0] ? --sq : ++sq) {
                    blocking_squares.push_back(sq);
                }
            }
            else if (is_inSameColumn(giving_check[0], sq_b_king)) {
                for (auto sq = sq_b_king;
                     sq != giving_check[0];
                     sq_b_king > giving_check[0] ? sq = sq - 8 : sq = sq + 8) {
                    blocking_squares.push_back(sq);
                }
            }
            else if (is_inSameDiagonal_leftRight(giving_check[0], sq_b_king)) {
                for (auto sq = sq_b_king;
                     sq != giving_check[0];
                     sq_b_king > giving_check[0] ? sq = sq - 7 : sq = sq + 7) {
                    blocking_squares.push_back(sq);
                }
            }
            else if (is_inSameDiagonal_rightLeft(giving_check[0], sq_b_king)) {
                for (auto sq = sq_b_king;
                     sq != giving_check[0];
                     sq_b_king > giving_check[0] ? sq = sq - 9 : sq = sq + 9) {
                    blocking_squares.push_back(sq);
                }
            }
        }
        for (auto& pair : maps->black_moves) {
            if (!is_king(pair.first)) {
                std::vector<Square> temp{};
                for (const auto& sq : pair.second) {
                    for (const auto& block_sq : blocking_squares) {
                        if (sq == block_sq) { temp.push_back(sq); }
                    }
                    if (sq == giving_check[0]) { temp.push_back(sq); }
                }
                pair.second = temp;
            }
        }
    }
        // if more than once piece is giving check, the king must be moved
    else if (giving_check.size() > 1) {
        // clear all moves except king evasion
        for (auto& pair : maps->black_moves) {
            if (!is_king(pair.first)) { pair.second.clear(); }
        }
    }

    // pinned piece reduction
    // white
    for (const auto& [pinned, pinner] : maps->white_pinned) {
        for (auto& [piece, moves] : maps->white_moves) {
            if (piece == pinned) {
                std::vector<Square> temp{};
                for (auto& sq : moves) {
                    if (is_inSameRow(sq_w_King, pinner, sq)) { temp.push_back(sq); }
                    if (is_inSameColumn(sq_w_King, pinner, sq)) { temp.push_back(sq); }
                    if (is_inSameDiagonal_leftRight(sq_w_King, pinner, sq)) { temp.push_back(sq); }
                    if (is_inSameDiagonal_rightLeft(sq_w_King, pinner, sq)) { temp.push_back(sq); }
                }
                moves = temp;
            }
        }
    }
    // black
    for (const auto& [pinned, pinner] : maps->black_pinned) {
        for (auto& [piece, moves] : maps->black_moves) {
            if (piece == pinned) {
                std::vector<Square> temp{};
                for (auto& sq : moves) {
                    if (is_inSameRow(sq_b_king, pinner, sq)) { temp.push_back(sq); }
                    if (is_inSameColumn(sq_b_king, pinner, sq)) { temp.push_back(sq); }
                    if (is_inSameDiagonal_leftRight(sq_b_king, pinner, sq)) { temp.push_back(sq); }
                    if (is_inSameDiagonal_rightLeft(sq_b_king, pinner, sq)) { temp.push_back(sq); }
                }
                moves = temp;
            }
        }
    }
}

// END update move maps
//----------------------------------------------------------------------------------------------------------------------
// BEGIN move

/**
 * @brief Move a pawn.
 * @details Check for promotion, two square move, or en passant, then move the piece.
 * @param from The square from which the pawn is moved.
 * @param to The square to which the pawn is moved.
 * @param ch If promoting, this gives the piece to promote to.
 */
void Board::move_pawn(Square from, Square to, char ch)
{
    bool promoted = false;

    // promotion
    if (ch == 'q' || ch == 'r' || ch == 'b' || ch == 'n') {
        Color c = what_color(from);
        if ((is_white_pawn(from) && get_row(to) == 8) || (is_black_pawn(from) && get_row(to) == 1)) { promoted = true; }
        if (promoted) {
            remove_piece(to);
            place_piece(to, c == Color::white ? static_cast<char>(std::toupper(ch)) : static_cast<char>(ch));
            remove_piece(from);
        }
    }
        // update en passant square
        // comes first because 'from' will be emptied after the move
    else if (!promoted) {
        if (is_white_pawn(from)) {
            if (static_cast<int>(to) - static_cast<int>(from) == 16) {
                game_state.en_passant_target = Sq::square_to_string(from + 8);
                game_state.en_passant_set = true;
            }
            else if (Sq::square_to_string(to) == game_state.en_passant_target) {
                remove_piece(Sq::string_to_square(game_state.en_passant_target) - 8);
            }
        }
        else if (is_black_pawn(from)) {
            if (static_cast<int>(from) - static_cast<int>(to) == 16) {
                game_state.en_passant_target = Sq::square_to_string(from - 8);
                game_state.en_passant_set = true;
            }
            else if (Sq::square_to_string(to) == game_state.en_passant_target) {
                remove_piece(Sq::string_to_square(game_state.en_passant_target) + 8);
            }
        }

        // move piece
        remove_piece(to);
        place_piece(to, what_piece(from));
        remove_piece(from);
    }
}

/**
 * @brief Move a king.
 * Check for castling, update castling rights, move the king.
 * @param from The square from which the king is moved.
 * @param to The square to which the king is moved.
 */
void Board::move_king(Square from, Square to)
{
    bool castled = false;

    // check for castling
    if (is_white_king(from) && from == Square::e1) {
        if (game_state.castle_w_K && to == Square::g1 && is_white_rook(Square::h1)) {
            remove_piece(to);
            place_piece(to, what_piece(from));
            remove_piece(from);
            move_rook(Square::h1, Square::f1);
            castled = true;
            game_state.castle_w_K = game_state.castle_w_Q = false;
        }
        else if (game_state.castle_w_Q && to == Square::c1 && is_white_rook(Square::a1)) {
            remove_piece(to);
            place_piece(to, what_piece(from));
            remove_piece(from);
            move_rook(Square::a1, Square::d1);
            castled = true;
            game_state.castle_w_K = game_state.castle_w_Q = false;
        }
    }
    if (is_black_king(from) && from == Square::e8) {
        if (game_state.castle_b_k && to == Square::g8 && is_black_rook(Square::h8)) {
            remove_piece(to);
            place_piece(to, what_piece(from));
            remove_piece(from);
            move_rook(Square::h8, Square::f8);
            castled = true;
            game_state.castle_b_k = game_state.castle_b_q = false;
        }
        else if (game_state.castle_b_q && to == Square::c8 && is_black_rook(Square::a8)) {
            remove_piece(to);
            place_piece(to, what_piece(from));
            remove_piece(from);
            move_rook(Square::a8, Square::d8);
            castled = true;
            game_state.castle_b_k = game_state.castle_b_q = false;
        }
    }

    // update castling rights
    if (is_white_king(from)) { game_state.castle_w_K = game_state.castle_w_Q = false; }
    else if (is_black_king(from)) { game_state.castle_b_k = game_state.castle_b_q = false; }

    // move piece
    if (!castled) {
        remove_piece(to);
        place_piece(to, what_piece(from));
        remove_piece(from);
    }
}

/**
 * @brief Move a rook
 * Update castling rights, then move the piece.
 * @param from The square from which the rook is moved
 * @param to The square to which tht rook is moved
 */
void Board::move_rook(Square from, Square to)
{
    // update castling rights
    if (game_state.castle_b_q && from == Square::a8 && is_black_rook(from)) { game_state.castle_b_q = false; }
    else if (game_state.castle_b_k && from == Square::h8 && is_black_rook(from)) { game_state.castle_b_k = false; }
    else if (game_state.castle_w_Q && from == Square::a1 && is_white_rook(from)) { game_state.castle_w_Q = false; }
    else if (game_state.castle_w_K && from == Square::h1 && is_white_rook(from)) { game_state.castle_w_K = false; }

    // move piece
    remove_piece(to);
    place_piece(to, what_piece(from));
    remove_piece(from);
}

/**
 * @brief Move a knight, bishop, or queen.
 * @details There are no special rules to consider when moving these pieces,
 * so we can simply pick them up and set them down
 * @param from The square from which the piece is moved.
 * @param to The square to which the piece is moved.
 */
void Board::move_piece(Square from, Square to)
{
    remove_piece(to);
    place_piece(to, what_piece(from));
    remove_piece(from);
}

/**
 * @brief Moves a piece on the chessboard
 * @param from The square from which the piece is moved.
 * @param to The square to which the piece is moved.
 * @param ch If a pawn is promoting, this gives the piece to promote to.
 * @see void Board::move_pawn(Square from, Square to, char ch)
 * @see void Board::move_king(Square from, Square to)
 * @see void Board::move_rook(Square from, Square to)
 * @see void Board::move_piece(Square from, Square to)
 */
void Board::move(Square from, Square to, char ch)
{
    // game state updates
    // comes first because from and to will change occupants after the move
    !game_state.active_color;                               // swap active color
    if (is_black(from)) { game_state.full_move_number++; }  // increment full move every time black moves

    // 50 move rule: draw if no pawn move or capture for 50 moves
    if (is_pawn(from) || ((what_color(to) != Color::none && !is_same_color(from, what_color(to))))) {
        game_state.half_move_clock = 0;
    }
    else { game_state.half_move_clock++; }

    // update castling rights if capturing rook
    if (is_white_rook(to)) {
        if (to == Square::h1) { game_state.castle_w_K = false; }
        else if (to == Square::a1) { game_state.castle_w_Q = false; }
    }
    else if (is_black_rook(to)) {
        if (to == Square::h8) { game_state.castle_b_k = false; }
        else if (to == Square::a8) { game_state.castle_b_q = false; }
    }

    // move piece
    if (is_pawn(from)) { move_pawn(from, to, ch); }
    else if (is_king(from)) { move_king(from, to); }
    else if (is_rook(from)) { move_rook(from, to); }
    else if (!is_empty(from)) { move_piece(from, to); }    // for knights, bishops, queens

    // en passant expires
    if (!game_state.en_passant_set && !game_state.en_passant_target.empty()) {
        game_state.en_passant_target.clear();
    }
    if (game_state.en_passant_set) { game_state.en_passant_set = false; }
}

// END move
//----------------------------------------------------------------------------------------------------------------------
// BEGIN diagnostic

/**
 * Calculates the number of nodes at depth 1 for a given color.
 * @param color The color of the pieces.
 * @return The number of nodes at depth 1.
 * @note This is only in use in a test file sample-game.cxx
 */
ulong Board::nodes_at_depth_1(Color color)
{
    update_move_maps();
    ulong nodes{};
    for (const auto& [sq, moves] : color == Color::white ? maps->white_moves : maps->black_moves) {
        nodes += moves.size();
    }
    return nodes;
}
