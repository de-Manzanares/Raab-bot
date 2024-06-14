#include "../include/Board.h"

using s = Square;

std::vector<std::vector<Square>> Diagonals::left_right = {
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
std::vector<std::vector<Square>> Diagonals::right_left = {
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

// END Boundary detection
//----------------------------------------------------------------------------------------------------------------------

void Maps::clear()
{
    move_map_white.clear();
    move_map_black.clear();
    influence_map_white.clear();
    influence_map_black.clear();
    pinned_pieces_white.clear();
    pinned_pieces_black.clear();
    pinned_piece_lane_map_white.clear();
    pinned_piece_lane_map_black.clear();
}

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

Board::~Board()
{
    delete maps;
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

// END clear
//----------------------------------------------------------------------------------------------------------------------


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
 * @brief Get the piece on the specified square
 * @param sq The square to check
 * @return char The piece on the square, ' ' if empty
 */
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
    if (w_Pawn & (1ULL << static_cast<int>(sq))) { return true; }
    if (w_Night & (1ULL << static_cast<int>(sq))) { return true; }
    if (w_Bishop & (1ULL << static_cast<int>(sq))) { return true; }
    if (w_Rook & (1ULL << static_cast<int>(sq))) { return true; }
    if (w_Queen & (1ULL << static_cast<int>(sq))) { return true; }
    if (w_King & (1ULL << static_cast<int>(sq))) { return true; }
    return false;
}

bool Board::is_black(Square sq) const
{
    if (b_pawn & (1ULL << static_cast<int>(sq))) { return true; }
    if (b_night & (1ULL << static_cast<int>(sq))) { return true; }
    if (b_bishop & (1ULL << static_cast<int>(sq))) { return true; }
    if (b_rook & (1ULL << static_cast<int>(sq))) { return true; }
    if (b_queen & (1ULL << static_cast<int>(sq))) { return true; }
    if (b_king & (1ULL << static_cast<int>(sq))) { return true; }
    return false;
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

bool Board::is_opposite_king(Square sq, Color c) const
{
    return c == Color::white ? is_black_king(sq) : is_white_king(sq);
}

int Board::is_in_row(Square sq)
{
    using s = Square;
    if (sq >= s::h1 && sq <= s::a1) { return 1; }
    if (sq >= s::h2 && sq <= s::a2) { return 2; }
    if (sq >= s::h3 && sq <= s::a3) { return 3; }
    if (sq >= s::h4 && sq <= s::a4) { return 4; }
    if (sq >= s::h5 && sq <= s::a5) { return 5; }
    if (sq >= s::h6 && sq <= s::a6) { return 6; }
    if (sq >= s::h7 && sq <= s::a7) { return 7; }
    if (sq >= s::h8 && sq <= s::a8) { return 8; }
}

int Board::is_in_row(Square sq) const
{
    using s = Square;
    if (sq >= s::h1 && sq <= s::a1) { return 1; }
    if (sq >= s::h2 && sq <= s::a2) { return 2; }
    if (sq >= s::h3 && sq <= s::a3) { return 3; }
    if (sq >= s::h4 && sq <= s::a4) { return 4; }
    if (sq >= s::h5 && sq <= s::a5) { return 5; }
    if (sq >= s::h6 && sq <= s::a6) { return 6; }
    if (sq >= s::h7 && sq <= s::a7) { return 7; }
    if (sq >= s::h8 && sq <= s::a8) { return 8; }
}

int Board::is_in_column(Square sq)
{
    using s = Square;
    if (static_cast<int>(sq) % 8 == 0) { return 8; }
    if (static_cast<int>(sq) % 8 == 1) { return 7; }
    if (static_cast<int>(sq) % 8 == 2) { return 6; }
    if (static_cast<int>(sq) % 8 == 3) { return 5; }
    if (static_cast<int>(sq) % 8 == 4) { return 4; }
    if (static_cast<int>(sq) % 8 == 5) { return 3; }
    if (static_cast<int>(sq) % 8 == 6) { return 2; }
    if (static_cast<int>(sq) % 8 == 7) { return 1; }
}

bool Board::is_in_same_row(Square sq1, Square sq2)
{
    return is_in_row(sq1) == is_in_row(sq2);
}

bool Board::is_in_same_column(Square sq1, Square sq2)
{
    return is_in_column(sq1) == is_in_column(sq2);
}

bool Board::is_in_same_diagonal_left_right(Square sq1, Square sq2)
{
    for (const auto& v : Diagonals::left_right) {
        for (const auto& e : v) {
            if (sq1 == e) {
                for (const auto& e2 : v) { if (sq2 == e2) { return true; }}
            }
        }
    }
    return false;
}

bool Board::is_in_same_diagonal_right_left(Square sq1, Square sq2)
{

    for (const auto& v : Diagonals::right_left) {
        for (const auto& e : v) {
            if (sq1 == e) {
                for (const auto& e2 : v) { if (sq2 == e2) { return true; }}
            }
        }
    }
    return false;
}

bool Board::is_in_same_row(Square sq1, Square sq2, Square sq3)
{
    return is_in_same_row(sq1, sq2) && is_in_same_row(sq2, sq3);
}

bool Board::is_in_same_column(Square sq1, Square sq2, Square sq3)
{
    return is_in_same_column(sq1, sq2) && is_in_same_column(sq2, sq3);
}

bool Board::is_in_same_diagonal_left_right(Square sq1, Square sq2, Square sq3)
{
    return is_in_same_diagonal_left_right(sq1, sq2) && is_in_same_diagonal_left_right(sq2, sq3);
}

bool Board::is_in_same_diagonal_right_left(Square sq1, Square sq2, Square sq3)
{
    return is_in_same_diagonal_right_left(sq1, sq2) && is_in_same_diagonal_right_left(sq2, sq3);
}

// END piece detection
//----------------------------------------------------------------------------------------------------------------------
// BEGIN FEN

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
 * @param board  A pointer to the Board object.
 * @param fen    The FEN string to import.
 */
// TODO check if fen is valid before/during import
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
            if (en_passant && square == ept) { if (is_in_row(ept) == 3 && is_black_pawn(sq)) { moves.push_back(ept); }}
            if (en_passant && square == ept) { if (is_in_row(ept) == 6 && is_white_pawn(sq)) { moves.push_back(ept); }}
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
    using s = Square;
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
void Board::update_influence_maps()
{
    // reset the maps
    maps->influence_map_white.clear();
    maps->influence_map_black.clear();

    for (Square square = Square::a8; square >= Square::h1; --square) {
        // white moves
        if (is_white(square)) {
            maps->influence_map_white.insert({square, influence(square)});
        }
        // black moves
        if (is_black(square)) {
            maps->influence_map_black.insert({square, influence(square)});
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
    bool found_one{};                 // xray through ONE enemy piece
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

void Board::update_pinned_pieces(const Square& square_K, const Square& square_k)
{
    // reset the maps
    maps->pinned_pieces_white.clear();
    maps->pinned_pieces_black.clear();
    maps->pinned_piece_lane_map_white.clear();
    maps->pinned_piece_lane_map_black.clear();

    for (Square square = Square::a8; square >= Square::h1; --square) {
        // white moves
        if (is_white(square)) {
            if (pinned_piece(square) != square) { maps->pinned_pieces_black.insert({pinned_piece(square), square}); }
        }
        // black moves
        if (is_black(square)) {
            if (pinned_piece(square) != square) { maps->pinned_pieces_white.insert({pinned_piece(square), square}); }
        }
    }
}

// END update pinned pieces
//----------------------------------------------------------------------------------------------------------------------
// BEGIN update king moves

/**
 * @brief Detects if the king is in check and updates the legal moves
 * @details This function detects if the king is in check and updates the legal moves.
 * It returns a vector of squares from which the king is giving check. It works with a single king on the board.
 * @param square_K The current position of the white king.
 * @return A vector of squares from which the king is giving check.
 * @warning only one king per board
 **/
std::vector<Square> Board::update_white_king_moves(Square square_K)
{
    using s = Square;
    using c = Color;

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
    influence_kings = influence(square_K);

    // detect whether king is in check
    is_in_check = false;
    for (const auto& pair : maps->influence_map_black) {
        if (std::find(pair.second.begin(), pair.second.end(), square_K) != pair.second.end()) {  // is the king in check
            is_in_check = true;
            game_state.in_check_white = true;
            // keep track of who is giving check
            giving_check.push_back(pair.first);
        }
    }
    if (!is_in_check) {     // if not in check, add castling moves (if available)
        if (game_state.castle_K) { influence_kings.push_back(s::g1); }
        if (game_state.castle_Q) { influence_kings.push_back(s::c1); }
    }

    // remove square that are under attack
    for (const auto& s : influence_kings) {
        under_attack = false;   // reset search flag
        for (const auto& pair : maps->influence_map_black) {
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
    if (game_state.castle_K) {
        if (std::find(legal_moves_kings.begin(), legal_moves_kings.end(), s::g1) != legal_moves_kings.end()) {
            if (std::find(legal_moves_kings.begin(), legal_moves_kings.end(), s::f1) == legal_moves_kings.end()) {
                // remove g1
                auto it = std::remove(legal_moves_kings.begin(), legal_moves_kings.end(), s::g1);
                legal_moves_kings.erase(it, legal_moves_kings.end());
            }
        }
    }
    if (game_state.castle_Q) {
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
    maps->move_map_white.insert({square_K, legal_moves_kings});

    return giving_check;
}

std::vector<Square> Board::update_black_king_moves(Square square_k)
{
    using s = Square;
    using c = Color;

    std::vector<Square> giving_check{};
    bool is_in_check;                           // king is in check
    bool under_attack;                          // flags a square as under attack
    std::vector<Square> influence_kings;        // temp vector to create king move list
    std::vector<Square> safe_squares;           // temp vector to create king move list
    std::vector<Square> legal_moves_kings;      // temp vector to create king move list

    // get started
    influence_kings = influence(square_k);

    // detect whether king is in check
    is_in_check = false;
    for (const auto& pair : maps->influence_map_white) {
        if (std::find(pair.second.begin(), pair.second.end(), square_k) != pair.second.end()) {  // is the king in check
            is_in_check = true;
            game_state.in_check_black = true;
            giving_check.push_back(pair.first);
        }
    }
    if (!is_in_check) {     // if not in check, add castling moves (if available)
        if (game_state.castle_k) { influence_kings.push_back(s::g8); }
        if (game_state.castle_q) { influence_kings.push_back(s::c8); }
    }

    // remove square that are under attack
    for (const auto& s : influence_kings) {
        under_attack = false;   // reset search flag
        for (const auto& pair : maps->influence_map_white) {
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
    if (game_state.castle_k) {
        if (std::find(legal_moves_kings.begin(), legal_moves_kings.end(), s::g8) != legal_moves_kings.end()) {
            if (std::find(legal_moves_kings.begin(), legal_moves_kings.end(), s::f8) == legal_moves_kings.end()) {
                // remove g8
                auto it = std::remove(legal_moves_kings.begin(), legal_moves_kings.end(), s::g8);
                legal_moves_kings.erase(it, legal_moves_kings.end());
            }
        }
    }
    if (game_state.castle_q) {
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
    maps->move_map_black.insert({square_k, legal_moves_kings});

    return giving_check;
}
// END update king moves
//----------------------------------------------------------------------------------------------------------------------
// BEGIN update move maps

/**
 * @brief Assigns squares from one unordered_map to another, excluding pawns and kings
 * @details This function iterates over the squares in the 'from' unordered_map and copies them to the 'to'
 * unordered_map, excluding any squares contain pawns or kings. If a king square is found, it is stored in a separate
 * variable and is returned at the end of the function.
 * @param to A pointer to the unordered_map where the squares will be assigned
 * @param from A pointer to the unordered_map from where the squares will be copied
 * @return The square that represents the king, if found
 */
Square Board::assign_from_influence_map_exclude_pawns_and_kings(std::unordered_map<Square, std::vector<Square>> *to,
        std::unordered_map<Square, std::vector<Square>> *from) const
{
    Square square_king{};
    to->clear();
    for (const auto& pair : *from) {
        if (!is_pawn(pair.first) && !is_king(pair.first)) { to->insert(pair); }
        else if (is_king(pair.first)) { square_king = pair.first; }
    }
    return square_king;
}

/**
 * @brief Extracts legal moves from influence maps
 * @details It is not legal to capture your own pieces
 * @param map A pointer to the unordered_map storing squares and their corresponding vectors of squares
 * @param color The color of the squares to be removed
 */
void Board::remove_same_color_squares(std::unordered_map<Square, std::vector<Square>> *map, Color color) const
{
    std::unordered_map<Square, std::vector<Square>> temp;   // to reassign original after cleaning
    std::vector<Square> temp_squares{};                     // to store non-same-colored squares

    for (const auto& pair : *map) {
        temp_squares.clear();                               // reset temp vector
        // record every non-same-colored square
        for (const auto& square : pair.second) { if (!is_same_color(square, color)) { temp_squares.push_back(square); }}
        temp.insert({pair.first, temp_squares});            // insert into "scrubbed" map
    }
    *map = temp;                                            // reassign original map
}

void Board::update_move_maps()
{
    // clear move maps
    maps->clear();

    // reset check flags
    game_state.in_check_white = false;
    game_state.in_check_black = false;

    // update influence maps
    update_influence_maps();

    // assign from influence maps exclude pawns and kings
    Square square_K = assign_from_influence_map_exclude_pawns_and_kings(
            &maps->move_map_white, &maps->influence_map_white);
    Square square_k = assign_from_influence_map_exclude_pawns_and_kings(
            &maps->move_map_black, &maps->influence_map_black);

    // update pinned pieces
    update_pinned_pieces(square_K, square_k);

    // remove same colored squares (can't capture own pieces)
    remove_same_color_squares(&maps->move_map_white, Color::white);
    remove_same_color_squares(&maps->move_map_black, Color::black);

    // add pawn moves directly
    for (auto sq = Square::a8; sq >= Square::h1; --sq) {
        if (is_white_pawn(sq)) { maps->move_map_white.insert({sq, legal_moves(sq)}); }
        if (is_black_pawn(sq)) { maps->move_map_black.insert({sq, legal_moves(sq)}); }
    }

    // add king moves
    // if king is in check, go through and "scrub" the move map
    std::vector<Square> giving_check{};
    std::vector<Square> blocking_squares{};

    // white
    giving_check = update_white_king_moves(square_K);

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
            if (is_in_same_row(giving_check[0], square_K)) {
                for (auto sq = square_K; sq != giving_check[0]; square_K > giving_check[0] ? --sq : ++sq) {
                    blocking_squares.push_back(sq);
                }
            }
            else if (is_in_same_column(giving_check[0], square_K)) {
                for (auto sq = square_K;
                     sq != giving_check[0];
                     square_K > giving_check[0] ? sq = sq - 8 : sq = sq + 8) {
                    blocking_squares.push_back(sq);
                }
            }
            else if (is_in_same_diagonal_left_right(giving_check[0], square_K)) {
                for (auto sq = square_K;
                     sq != giving_check[0];
                     square_K > giving_check[0] ? sq = sq - 7 : sq = sq + 7) {
                    blocking_squares.push_back(sq);
                }
            }
            else if (is_in_same_diagonal_right_left(giving_check[0], square_K)) {
                for (auto sq = square_K;
                     sq != giving_check[0];
                     square_K > giving_check[0] ? sq = sq - 9 : sq = sq + 9) {
                    blocking_squares.push_back(sq);
                }
            }
        }
        for (auto& pair : maps->move_map_white) {
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
        for (auto& pair : maps->move_map_white) {
            if (!is_king(pair.first)) { pair.second.clear(); }
        }
    }

    // black
    giving_check.clear();
    giving_check = update_black_king_moves(square_k);
    blocking_squares.clear();   // reset
    // if one piece is giving check, it can be captured or blocked
    if (giving_check.size() == 1) {
        if (!is_knight(giving_check[0]) && !is_pawn(giving_check[0])) {
            if (is_in_same_row(giving_check[0], square_k)) {
                for (auto sq = square_k; sq != giving_check[0]; square_k > giving_check[0] ? --sq : ++sq) {
                    blocking_squares.push_back(sq);
                }
            }
            else if (is_in_same_column(giving_check[0], square_k)) {
                for (auto sq = square_k;
                     sq != giving_check[0];
                     square_k > giving_check[0] ? sq = sq - 8 : sq = sq + 8) {
                    blocking_squares.push_back(sq);
                }
            }
            else if (is_in_same_diagonal_left_right(giving_check[0], square_k)) {
                for (auto sq = square_k;
                     sq != giving_check[0];
                     square_k > giving_check[0] ? sq = sq - 7 : sq = sq + 7) {
                    blocking_squares.push_back(sq);
                }
            }
            else if (is_in_same_diagonal_right_left(giving_check[0], square_k)) {
                for (auto sq = square_k;
                     sq != giving_check[0];
                     square_k > giving_check[0] ? sq = sq - 9 : sq = sq + 9) {
                    blocking_squares.push_back(sq);
                }
            }
        }
        for (auto& pair : maps->move_map_black) {
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
        for (auto& pair : maps->move_map_black) {
            if (!is_king(pair.first)) { pair.second.clear(); }
        }
    }

    // pinned piece reduction
    // white
    for (const auto& [pinned, pinner] : maps->pinned_pieces_white) {
        for (auto& [key, squares] : maps->move_map_white) {
            if (key == pinned) {
                std::vector<Square> temp{};
                for (auto& square : squares) {
                    if (is_in_same_row(square_K, pinner, square)) { temp.push_back(square); }
                    if (is_in_same_column(square_K, pinner, square)) { temp.push_back(square); }
                    if (is_in_same_diagonal_left_right(square_K, pinner, square)) { temp.push_back(square); }
                    if (is_in_same_diagonal_right_left(square_K, pinner, square)) { temp.push_back(square); }
                }
                squares = temp;
            }
        }
    }
    // black
    for (const auto& [pinned, pinner] : maps->pinned_pieces_black) {
        for (auto& [key, squares] : maps->move_map_black) {
            if (key == pinned) {
                std::vector<Square> temp{};
                for (auto& square : squares) {
                    if (is_in_same_row(square_k, pinner, square)) { temp.push_back(square); }
                    if (is_in_same_column(square_k, pinner, square)) { temp.push_back(square); }
                    if (is_in_same_diagonal_left_right(square_k, pinner, square)) { temp.push_back(square); }
                    if (is_in_same_diagonal_right_left(square_k, pinner, square)) { temp.push_back(square); }
                }
                squares = temp;
            }
        }
    }
}

// END update move maps
//----------------------------------------------------------------------------------------------------------------------
// BEGIN move

void Board::move_pawn(Square from, Square to, char ch)
{
    bool promoted = false;

    // promotion
    if (ch == 'q' || ch == 'r' || ch == 'b' || ch == 'n') {
        Color c = what_color(from);
        if (is_white_pawn(from) && is_in_row(to) == 8) { promoted = true; }
        else if (is_black_pawn(from) && is_in_row(to) == 1) { promoted = true; }
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

void Board::move_king(Square from, Square to)
{
    bool castled = false;
    if (is_black_king(from) && from == Square::e8) {
        if (to == Square::g8 && game_state.castle_k && is_black_rook(Square::h8)) {
            remove_piece(to);
            place_piece(to, what_piece(from));
            remove_piece(from);
            move_rook(Square::h8, Square::f8);
            castled = true;
            game_state.castle_k = game_state.castle_q = false;
        }
        else if (to == Square::c8 && game_state.castle_q && is_black_rook(Square::a8)) {
            remove_piece(to);
            place_piece(to, what_piece(from));
            remove_piece(from);
            move_rook(Square::a8, Square::d8);
            castled = true;
            game_state.castle_k = game_state.castle_q = false;
        }
    }
    if (is_white_king(from) && from == Square::e1) {
        if (to == Square::g1 && game_state.castle_K && is_white_rook(Square::h1)) {
            remove_piece(to);
            place_piece(to, what_piece(from));
            remove_piece(from);
            move_rook(Square::h1, Square::f1);
            castled = true;
            game_state.castle_K = game_state.castle_Q = false;
        }
        else if (to == Square::c1 && game_state.castle_Q && is_white_rook(Square::a1)) {
            remove_piece(to);
            place_piece(to, what_piece(from));
            remove_piece(from);
            move_rook(Square::a1, Square::d1);
            castled = true;
            game_state.castle_K = game_state.castle_Q = false;
        }
    }
    // update castling rights
    if (is_white_king(from)) { game_state.castle_K = game_state.castle_Q = false; }
    else if (is_black_king(from)) { game_state.castle_k = game_state.castle_q = false; }

    // move piece
    if (!castled) {
        remove_piece(to);
        place_piece(to, what_piece(from));
        remove_piece(from);
    }
}

void Board::move_rook(Square from, Square to)
{
    // update castling rights
    if (from == Square::a8 && is_black_rook(from) && game_state.castle_q) { game_state.castle_q = false; }
    else if (from == Square::h8 && is_black_rook(from) && game_state.castle_k) { game_state.castle_k = false; }
    else if (from == Square::a1 && is_white_rook(from) && game_state.castle_Q) { game_state.castle_Q = false; }
    else if (from == Square::h1 && is_white_rook(from) && game_state.castle_K) { game_state.castle_K = false; }

    // move piece
    remove_piece(to);
    place_piece(to, what_piece(from));
    remove_piece(from);
}

void Board::move_piece(Square from, Square to)
{
    remove_piece(to);
    place_piece(to, what_piece(from));
    remove_piece(from);
}

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
        if (to == Square::h1) { game_state.castle_K = false; }
        else if (to == Square::a1) { game_state.castle_Q = false; }
    }
    else if (is_black_rook(to)) {
        if (to == Square::h8) { game_state.castle_k = false; }
        else if (to == Square::a8) { game_state.castle_q = false; }
    }

    // move piece
    if (is_pawn(from)) { move_pawn(from, to, ch); }
    if (is_king(from)) { move_king(from, to); }
    if (is_rook(from)) { move_rook(from, to); }
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
void Board::print_move_map(Color color) const
{
    auto map = color == Color::white ? maps->move_map_white : maps->move_map_black;
    for (const auto& [sq, moves] : map) {
        std::cout << Sq::square_to_string(sq) << " : ";
        for (const auto& square : moves) {
            std::cout << Sq::square_to_string(square) << " ";
        }
        std::cout << "\n";
    }
}

ulong Board::nodes_at_depth_1(Color color)
{
    update_move_maps();
    ulong nodes{};
    for (const auto& [sq, moves] : color == Color::white ? maps->move_map_white : maps->move_map_black) {
        nodes += moves.size();
    }
    return nodes;
}
