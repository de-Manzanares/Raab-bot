#include "Board.h"

#include <algorithm>
#include <sstream>

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

//------------------------------------------------------------------------------
// BEGIN Boundary detection

bool is_upper_vertical_boundary(Square sq) { return static_cast<int>(sq) > 55; }

bool is_lower_vertical_boundary(Square sq) { return static_cast<int>(sq) < 8; }

bool is_left_horizontal_boundary(const Square sq) {
  return sq == s::a1 || sq == s::a2 || sq == s::a3 || sq == s::a4 ||
         sq == s::a5 || sq == s::a6 || sq == s::a7 || sq == s::a8;
}

bool is_right_horizontal_boundary(const Square sq) {
  return sq == s::h1 || sq == s::h2 || sq == s::h3 || sq == s::h4 ||
         sq == s::h5 || sq == s::h6 || sq == s::h7 || sq == s::h8;
}

// END Boundary detection
//------------------------------------------------------------------------------

void Maps::clear() {
  white_influence.clear();
  black_influence.clear();
  white_pinned.clear();
  black_pinned.clear();
  white_moves.clear();
  black_moves.clear();
}

Board::~Board() { delete maps; }

Board &Board::operator=(const Board &rhs) {
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

void Board::clear() {
  // remove all the pieces
  b_pawn = b_night = b_bishop = b_rook = b_queen = b_king = 0;
  w_Pawn = w_Night = w_Bishop = w_Rook = w_Queen = w_King = 0;
  // clear the game state
  game_state.clear();
}

void Board::remove_piece(Square square) {
  const uint64_t mask = ~(1ULL << static_cast<uint>(square));
  for (auto &[ch, bits] : piece_map) {
    bits &= mask;
  }
}

void Board::place_piece(Square square, const char &ch) const {
  for (const auto &[fst, snd] : piece_map) {
    if (ch == fst) {
      snd |= 1ULL << static_cast<uint>(square);
    }
  }
}

//------------------------------------------------------------------------------
// BEGIN piece detection

char Board::what_piece(Square sq) const {
  for (const auto &[key, value] : piece_map) {
    if (value & 1ULL << static_cast<int>(sq)) {
      return key;
    }
  }
  return ' ';
}

bool Board::is_white_rook(Square sq) const {
  return w_Rook & 1ULL << static_cast<int>(sq);
}

bool Board::is_black_rook(Square sq) const {
  return b_rook & 1ULL << static_cast<int>(sq);
}

bool Board::is_rook(const Square sq) const {
  return is_white_rook(sq) || is_black_rook(sq);
}

bool Board::is_white_bishop(Square sq) const {
  return w_Bishop & 1ULL << static_cast<int>(sq);
}

bool Board::is_black_bishop(Square sq) const {
  return b_bishop & 1ULL << static_cast<int>(sq);
}

bool Board::is_bishop(const Square sq) const {
  return is_white_bishop(sq) || is_black_bishop(sq);
}

bool Board::is_white_queen(Square sq) const {
  return w_Queen & 1ULL << static_cast<int>(sq);
}

bool Board::is_black_queen(Square sq) const {
  return b_queen & 1ULL << static_cast<int>(sq);
}

bool Board::is_queen(const Square sq) const {
  return is_white_queen(sq) || is_black_queen(sq);
}

bool Board::is_white_knight(Square sq) const {
  return w_Night & 1ULL << static_cast<int>(sq);
}

bool Board::is_black_knight(Square sq) const {
  return b_night & 1ULL << static_cast<int>(sq);
}

bool Board::is_knight(const Square sq) const {
  return is_white_knight(sq) || is_black_knight(sq);
}

bool Board::is_white_king(Square sq) const {
  return w_King & 1ULL << static_cast<int>(sq);
}

bool Board::is_black_king(Square sq) const {
  return b_king & 1ULL << static_cast<int>(sq);
}

bool Board::is_king(const Square sq) const {
  return is_white_king(sq) || is_black_king(sq);
}

bool Board::is_white_pawn(Square sq) const {
  return w_Pawn & 1ULL << static_cast<int>(sq);
}

bool Board::is_black_pawn(Square sq) const {
  return b_pawn & 1ULL << static_cast<int>(sq);
}

bool Board::is_pawn(const Square sq) const {
  return is_white_pawn(sq) || is_black_pawn(sq);
}

bool Board::is_white(const Square sq) const {
  return is_white_pawn(sq) || is_white_king(sq) || is_white_knight(sq) ||
         is_white_bishop(sq) || is_white_rook(sq) || is_white_queen(sq);
}

bool Board::is_black(const Square sq) const {
  return is_black_pawn(sq) || is_black_king(sq) || is_black_knight(sq) ||
         is_black_bishop(sq) || is_black_rook(sq) || is_black_queen(sq);
}

Color Board::what_color(const Square sq) const {
  if (is_white(sq)) {
    return Color::white;
  }
  if (is_black(sq)) {
    return Color::black;
  }
  return Color::none;
}

bool Board::is_same_color(const Square sq, const Color color) const {
  return what_color(sq) == color;
}

bool Board::is_empty(const Square sq) const { return what_piece(sq) == ' '; }

bool Board::is_opposite_king(const Square sq, const Color c) const {
  return c == Color::white ? is_black_king(sq) : is_white_king(sq);
}

int Board::get_row(const Square sq) {
  if (sq >= s::h1 && sq <= s::a1) {
    return 1;
  }
  if (sq >= s::h2 && sq <= s::a2) {
    return 2;
  }
  if (sq >= s::h3 && sq <= s::a3) {
    return 3;
  }
  if (sq >= s::h4 && sq <= s::a4) {
    return 4;
  }
  if (sq >= s::h5 && sq <= s::a5) {
    return 5;
  }
  if (sq >= s::h6 && sq <= s::a6) {
    return 6;
  }
  if (sq >= s::h7 && sq <= s::a7) {
    return 7;
  }
  if (sq >= s::h8 && sq <= s::a8) {
    return 8;
  }
  return 0;
}

int Board::get_column(Square sq) {
  if (static_cast<int>(sq) % 8 == 0) {
    return 8;
  }
  if (static_cast<int>(sq) % 8 == 1) {
    return 7;
  }
  if (static_cast<int>(sq) % 8 == 2) {
    return 6;
  }
  if (static_cast<int>(sq) % 8 == 3) {
    return 5;
  }
  if (static_cast<int>(sq) % 8 == 4) {
    return 4;
  }
  if (static_cast<int>(sq) % 8 == 5) {
    return 3;
  }
  if (static_cast<int>(sq) % 8 == 6) {
    return 2;
  }
  if (static_cast<int>(sq) % 8 == 7) {
    return 1;
  }
  return 0;
}

// TODO variadic templates?

bool Board::is_inSameRow(const Square sq1, const Square sq2) {
  return get_row(sq1) == get_row(sq2);
}

bool Board::is_inSameRow(const Square sq1, const Square sq2, const Square sq3) {
  return is_inSameRow(sq1, sq2) && is_inSameRow(sq2, sq3);
}

bool Board::is_inSameColumn(const Square sq1, const Square sq2) {
  return get_column(sq1) == get_column(sq2);
}

bool Board::is_inSameColumn(const Square sq1, const Square sq2,
                            const Square sq3) {
  return is_inSameColumn(sq1, sq2) && is_inSameColumn(sq2, sq3);
}

bool Board::is_inSameDiagonal_leftRight(const Square sq1, const Square sq2) {
  for (const auto &v : Diagonals::leftRight) {
    for (const auto &e : v) {
      if (sq1 == e) {
        for (const auto &e2 : v) {
          if (sq2 == e2) {
            return true;
          }
        }
      }
    }
  }
  return false;
}

bool Board::is_inSameDiagonal_leftRight(const Square sq1, const Square sq2,
                                        const Square sq3) {
  return is_inSameDiagonal_leftRight(sq1, sq2) &&
         is_inSameDiagonal_leftRight(sq2, sq3);
}

bool Board::is_inSameDiagonal_rightLeft(const Square sq1, const Square sq2) {
  for (const auto &v : Diagonals::rightLeft) {
    for (const auto &e : v) {
      if (sq1 == e) {
        for (const auto &e2 : v) {
          if (sq2 == e2) {
            return true;
          }
        }
      }
    }
  }
  return false;
}

bool Board::is_inSameDiagonal_rightLeft(const Square sq1, const Square sq2,
                                        const Square sq3) {
  return is_inSameDiagonal_rightLeft(sq1, sq2) &&
         is_inSameDiagonal_rightLeft(sq2, sq3);
}

// END piece detection
//------------------------------------------------------------------------------
// BEGIN FEN

namespace fen_piece_placement {
bool isBitSet(const uint64_t n, Square sq) {
  return n & 1ULL << static_cast<int>(sq);
}
} // namespace fen_piece_placement

std::string Board::fen_piece_placement() const {
  std::string piece_placement;
  int emptySquares = 0; // to count empty squares
  for (auto sq = s::a8; sq >= s::h1; --sq) {
    bool pieceFound = false;
    // search the square for each type of piece
    for (const auto &[fst, snd] : piece_map) {
      if (fen_piece_placement::isBitSet(snd, sq)) {
        if (emptySquares > 0) {
          piece_placement += std::to_string(emptySquares);
          emptySquares = 0;
        }
        piece_placement += fst;
        pieceFound = true;
        break;
      }
    }
    // count empty squares
    if (!pieceFound) {
      emptySquares++;
      // to flush last empty squares
      if (sq == s::h1) {
        piece_placement += std::to_string(emptySquares);
        emptySquares = 0;
      }
    }
    // slash for each new row (excluding an eighth slash)
    if (sq != s::h1 && static_cast<int>(sq) % 8 == 0) {
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

std::string Board::export_fen() const {
  std::string fen; // string to be exported
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

namespace set_pieces {
bool is_piece(const char &ch) {
  return ch == 'p' || ch == 'n' || ch == 'b' || ch == 'r' || ch == 'q' ||
         ch == 'k' || ch == 'P' || ch == 'N' || ch == 'B' || ch == 'R' ||
         ch == 'Q' || ch == 'K';
}
} // namespace set_pieces

uint Board::set_pieces(const std::string &fen) const {
  auto square = s::a8;
  uint counter{};
  for (const auto &ch : fen) {
    if (set_pieces::is_piece(ch)) {
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

void Board::import_fen(const std::string &fen) {
  std::string temp;

  // clear the board
  clear();

  // set pieces and create game state string
  const std::string game_state_string = fen.substr(set_pieces(fen));

  // set games state variables
  std::istringstream iss(game_state_string);

  // create variables
  char active_color;
  std::string castling_ability;
  std::string en_passant_target;
  uint half_move_clock;
  uint full_move_number;

  // gather data
  iss >> active_color >> castling_ability >> en_passant_target >>
      half_move_clock >> full_move_number;

  // assign to board variables
  // active color
  active_color == 'w' ? game_state.active_color = Color::white
                      : game_state.active_color = Color::black;

  // castling ability
  game_state.set_castling_ability(castling_ability);
  game_state.en_passant_target = en_passant_target;
  game_state.half_move_clock = half_move_clock;
  game_state.full_move_number = full_move_number;
}

// END FEN
//------------------------------------------------------------------------------
// BEGIN legal moves

std::vector<Square> Board::legal_moves_pawn(Square sq) const {
  std::vector<Square> pawn_moves{};
  if (is_white_pawn(sq)) {
    const int isq = static_cast<int>(sq);
    const bool starting_row = isq <= 15 && isq >= 8;
    if (is_empty(sq + 8)) {
      pawn_moves.push_back(sq + 8);
    }
    if (starting_row && is_empty(sq + 8) && is_empty(sq + 16)) {
      pawn_moves.push_back(sq + 16);
    }
  }
  if (is_black_pawn(sq)) {
    const int isq = static_cast<int>(sq);
    const bool starting_row = isq <= 55 && isq >= 48;
    if (is_empty(sq - 8)) {
      pawn_moves.push_back(sq - 8);
    }
    if (starting_row && is_empty(sq - 8) && is_empty(sq - 16)) {
      pawn_moves.push_back(sq - 16);
    }
  }
  return pawn_moves;
}

std::vector<Square> Board::legal_moves(const Square sq) const {
  std::vector<Square> moves{};
  std::vector<Square> temp = influence(sq);

  if (!is_pawn(sq)) {
    for (const auto &square : temp) {
      if (!is_same_color(sq, what_color(square))) {
        moves.push_back(square);
      }
    }
  }
  if (is_pawn(sq)) {
    Square ept{};
    bool en_passant = false;
    if (!game_state.en_passant_target.empty()) {
      ept = Sq::string_to_square(game_state.en_passant_target);
      en_passant = true;
    }
    for (const auto &square : temp) {
      if (!is_same_color(sq, what_color(square)) && !is_empty(square)) {
        moves.push_back(square);
      }
      if (en_passant && square == ept) {
        if (get_row(ept) == 3 && is_black_pawn(sq)) {
          moves.push_back(ept);
        }
      }
      if (en_passant && square == ept) {
        if (get_row(ept) == 6 && is_white_pawn(sq)) {
          moves.push_back(ept);
        }
      }
    }
    for (std::vector<Square> temp1 = legal_moves_pawn(sq);
         const auto &square : temp1) {
      moves.push_back(square);
    }
  }
  return moves;
}

// END legal moves
//------------------------------------------------------------------------------
// BEGIN influence rook

std::vector<Square> Board::influence_rook(const Square sq) const {
  std::vector<Square> influence{};
  influence.reserve(16);
  // vertical up
  for (auto square = sq + 8; !is_upper_vertical_boundary(square - 8);
       square = square + 8) {
    influence.push_back(square);
    if (!is_empty(square) && !is_opposite_king(square, what_color(sq))) {
      break;
    }
  }
  // vertical down
  for (auto square = sq - 8; !is_lower_vertical_boundary(square + 8);
       square = square - 8) {
    influence.push_back(square);
    if (!is_empty(square) && !is_opposite_king(square, what_color(sq))) {
      break;
    }
  }
  // horizontal right
  for (auto square = sq - 1; !is_right_horizontal_boundary(square + 1);
       square = square - 1) {
    influence.push_back(square);
    if (!is_empty(square) && !is_opposite_king(square, what_color(sq))) {
      break;
    }
  }
  // horizontal left
  for (auto square = sq + 1; !is_left_horizontal_boundary(square - 1);
       square = square + 1) {
    influence.push_back(square);
    if (!is_empty(square) && !is_opposite_king(square, what_color(sq))) {
      break;
    }
  }
  return influence;
}

// END influence rook
//------------------------------------------------------------------------------
// BEGIN influence bishop

std::vector<Square> Board::influence_bishop(const Square sq) const {
  std::vector<Square> influence{};
  influence.reserve(16);
  // up left
  for (auto square = sq + 9; !is_upper_vertical_boundary(square - 9) &&
                             !is_left_horizontal_boundary(square - 9);
       square = square + 9) {
    influence.push_back(square);
    if (!is_empty(square) && !is_opposite_king(square, what_color(sq))) {
      break;
    }
  }
  // up right
  for (auto square = sq + 7; !is_upper_vertical_boundary(square - 7) &&
                             !is_right_horizontal_boundary(square - 7);
       square = square + 7) {
    influence.push_back(square);
    if (!is_empty(square) && !is_opposite_king(square, what_color(sq))) {
      break;
    }
  }
  // down right
  for (auto square = sq - 9; !is_lower_vertical_boundary(square + 9) &&
                             !is_right_horizontal_boundary(square + 9);
       square = square - 9) {
    influence.push_back(square);
    if (!is_empty(square) && !is_opposite_king(square, what_color(sq))) {
      break;
    }
  }
  // down left
  for (auto square = sq - 7; !is_lower_vertical_boundary(square + 7) &&
                             !is_left_horizontal_boundary(square + 7);
       square = square - 7) {
    influence.push_back(square);
    if (!is_empty(square) && !is_opposite_king(square, what_color(sq))) {
      break;
    }
  }
  return influence;
}

// END influence bishop
//------------------------------------------------------------------------------
// BEGIN influence queen

std::vector<Square> Board::influence_queen(const Square sq) const {
  std::vector<Square> v1 = influence_rook(sq);
  std::vector<Square> v2 = influence_bishop(sq);
  std::vector<Square> influence;
  influence.reserve(32);
  influence.insert(influence.end(), v1.begin(), v1.end());
  influence.insert(influence.end(), v2.begin(), v2.end());
  return influence;
}

// END influence queen
//------------------------------------------------------------------------------
// BEGIN influence knight

std::vector<Square> Board::influence_knight(Square sq) {
  std::vector<Square> influence{};
  influence.reserve(8);

  // center 4x4 - 16 squares
  if (const int isq = static_cast<int>(sq);
      isq >= 18 && isq <= 21 || isq >= 26 && isq <= 29 ||
      isq >= 34 && isq <= 37 || isq >= 42 && isq <= 45) {
    influence.push_back(sq - (2 * 8 + 1)); // down right
    influence.push_back(sq - (2 * 8 - 1)); // down left
    influence.push_back(sq - (8 + 2));     // right down
    influence.push_back(sq - (8 - 2));     // left down
    influence.push_back(sq + (8 - 2));     // right up
    influence.push_back(sq + (8 + 2));     // left up
    influence.push_back(sq + (2 * 8 - 1)); // up right
    influence.push_back(sq + (2 * 8 + 1)); // up left
  }
  // b file - 4 squares
  else if (sq == s::b3 || sq == s::b4 || sq == s::b5 || sq == s::b6) {
    influence.push_back(sq - (2 * 8 + 1)); // down right
    influence.push_back(sq - (2 * 8 - 1)); // down left
    influence.push_back(sq - (8 + 2));     // right down
    influence.push_back(sq + (8 - 2));     // right up
    influence.push_back(sq + (2 * 8 - 1)); // up right
    influence.push_back(sq + (2 * 8 + 1)); // up left
  }
  // g files - 4 squares
  else if (sq == s::g3 || sq == s::g4 || sq == s::g5 || sq == s::g6) {
    influence.push_back(sq - (2 * 8 + 1)); // down right
    influence.push_back(sq - (2 * 8 - 1)); // down left
    influence.push_back(sq - (8 - 2));     // left down
    influence.push_back(sq + (8 + 2));     // left up
    influence.push_back(sq + (2 * 8 - 1)); // up right
    influence.push_back(sq + (2 * 8 + 1)); // up left
  }
  // 2nd rank - 4 squares
  else if (isq >= static_cast<int>(s::f2) && isq <= static_cast<int>(s::c2)) {
    influence.push_back(sq - (8 + 2));     // right down
    influence.push_back(sq - (8 - 2));     // left down
    influence.push_back(sq + (8 - 2));     // right up
    influence.push_back(sq + (8 + 2));     // left up
    influence.push_back(sq + (2 * 8 - 1)); // up right
    influence.push_back(sq + (2 * 8 + 1)); // up left
  }
  // 7th rank - 4 squares
  else if (isq >= static_cast<int>(s::f7) && isq <= static_cast<int>(s::c7)) {
    influence.push_back(sq - (2 * 8 + 1)); // down right
    influence.push_back(sq - (2 * 8 - 1)); // down left
    influence.push_back(sq - (8 + 2));     // right down
    influence.push_back(sq - (8 - 2));     // left down
    influence.push_back(sq + (8 - 2));     // right up
    influence.push_back(sq + (8 + 2));     // left up
  }
  // a file - 4 squares
  else if (sq == s::a3 || sq == s::a4 || sq == s::a5 || sq == s::a6) {
    influence.push_back(sq - (2 * 8 + 1)); // down right
    influence.push_back(sq - (8 + 2));     // right down
    influence.push_back(sq + (8 - 2));     // right up
    influence.push_back(sq + (2 * 8 - 1)); // up right
  }
  // h file - 4 squares
  else if (sq == s::h3 || sq == s::h4 || sq == s::h5 || sq == s::h6) {
    influence.push_back(sq - (2 * 8 - 1)); // down left
    influence.push_back(sq - (8 - 2));     // left down
    influence.push_back(sq + (8 + 2));     // left up
    influence.push_back(sq + (2 * 8 + 1)); // up left
  }
  // 1st rank - 4 squares
  else if (isq >= static_cast<int>(s::f1) && isq <= static_cast<int>(s::c1)) {
    influence.push_back(sq + (8 - 2));     // right up
    influence.push_back(sq + (8 + 2));     // left up
    influence.push_back(sq + (2 * 8 - 1)); // up right
    influence.push_back(sq + (2 * 8 + 1)); // up left
  }
  // 8th rank - 4 squares
  else if (isq >= static_cast<int>(s::f8) && isq <= static_cast<int>(s::c8)) {
    influence.push_back(sq - (2 * 8 + 1)); // down right
    influence.push_back(sq - (2 * 8 - 1)); // down left
    influence.push_back(sq - (8 + 2));     // right down
    influence.push_back(sq - (8 - 2));     // left down
  }
  // inside 2's and 7's - 1 square each
  else if (isq == static_cast<int>(s::g2)) {
    influence = {s::e1, s::e3, s::h4, s::f4};
  } else if (isq == static_cast<int>(s::b2)) {
    influence = {s::d1, s::d3, s::c4, s::a4};
  } else if (isq == static_cast<int>(s::g7)) {
    influence = {s::h5, s::f5, s::e6, s::e8};
  } else if (isq == static_cast<int>(s::b7)) {
    influence = {s::c5, s::a5, s::d6, s::d8};
  }
  // edge 2's and 7's - 1 square each
  else if (isq == static_cast<int>(s::h2)) {
    influence = {s::f1, s::f3, s::g4};
  } else if (isq == static_cast<int>(s::a2)) {
    influence = {s::c1, s::c3, s::b4};
  } else if (isq == static_cast<int>(s::h7)) {
    influence = {s::g5, s::f6, s::f8};
  } else if (isq == static_cast<int>(s::a7)) {
    influence = {s::b5, s::c6, s::c8};
  }
  // edge 1's and 8's - 1 square each
  else if (isq == static_cast<int>(s::g1)) {
    influence = {s::e2, s::h3, s::f3};
  } else if (isq == static_cast<int>(s::b1)) {
    influence = {s::d2, s::c3, s::a3};
  } else if (isq == static_cast<int>(s::g8)) {
    influence = {s::h6, s::f6, s::e7};
  } else if (isq == static_cast<int>(s::b8)) {
    influence = {s::c6, s::a6, s::d7};
  }
  // corners - 1 square each
  else if (isq == static_cast<int>(s::h1)) {
    influence = {s::f2, s::g3};
  } else if (isq == static_cast<int>(s::a1)) {
    influence = {s::c2, s::b3};
  } else if (isq == static_cast<int>(s::h8)) {
    influence = {s::g6, s::f7};
  } else if (isq == static_cast<int>(s::a8)) {
    influence = {s::b6, s::c7};
  }

  return influence;
}

// END influence knight
//------------------------------------------------------------------------------
// BEGIN influence king

std::vector<Square> Board::influence_king(const Square sq) {
  std::vector<Square> influence{};
  influence.reserve(16);
  // vertical
  if (!is_upper_vertical_boundary(sq)) {
    influence.push_back(sq + 8);
  }
  if (!is_lower_vertical_boundary(sq)) {
    influence.push_back(sq - 8);
  }
  // horizontal
  if (!is_right_horizontal_boundary(sq)) {
    influence.push_back(sq - 1);
  }
  if (!is_left_horizontal_boundary(sq)) {
    influence.push_back(sq + 1);
  }
  // diagonal
  if (!is_lower_vertical_boundary(sq) && !is_right_horizontal_boundary(sq)) {
    influence.push_back(sq - 9);
  }
  if (!is_lower_vertical_boundary(sq) && !is_left_horizontal_boundary(sq)) {
    influence.push_back(sq - 7);
  }
  if (!is_upper_vertical_boundary(sq) && !is_right_horizontal_boundary(sq)) {
    influence.push_back(sq + 7);
  }
  if (!is_upper_vertical_boundary(sq) && !is_left_horizontal_boundary(sq)) {
    influence.push_back(sq + 9);
  }

  return influence;
}

// END influence king
//------------------------------------------------------------------------------
// BEGIN influence pawn

std::vector<Square> Board::influence_pawn(const Square sq) const {
  std::vector<Square> influence{};
  influence.reserve(8);
  constexpr int front_left = 9;
  constexpr int front_right = 7;

  if (!is_left_horizontal_boundary(sq)) {
    is_white_pawn(sq) ? influence.push_back(sq + front_left)
                      : influence.push_back(sq - front_right);
  }
  if (!is_right_horizontal_boundary(sq)) {
    is_white_pawn(sq) ? influence.push_back(sq + front_right)
                      : influence.push_back(sq - front_left);
  }
  return influence;
}

// END influence pawn
//------------------------------------------------------------------------------
// BEGIN influence

std::vector<Square> Board::influence(const Square sq) const {
  std::vector<Square> influence{};
  influence.reserve(32);

  // if the square is empty, there is no influence
  if (what_piece(sq) == ' ') {
    return influence;
  }
  if (is_pawn(sq)) {
    influence = influence_pawn(sq);
  } else if (is_rook(sq)) {
    influence = influence_rook(sq);
  } else if (is_bishop(sq)) {
    influence = influence_bishop(sq);
  } else if (is_queen(sq)) {
    influence = influence_queen(sq);
  } else if (is_knight(sq)) {
    influence = influence_knight(sq);
  } else if (is_king(sq)) {
    influence = influence_king(sq);
  }
  return influence;
}

// END influence
//------------------------------------------------------------------------------
// BEGIN update influence maps

void Board::update_influence_maps() const {
  // reset the maps
  maps->white_influence.clear();
  maps->black_influence.clear();

  for (auto sq = s::a8; sq >= s::h1; --sq) {
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
//------------------------------------------------------------------------------
// BEGIN pinned piece rook

Square Board::pinned_piece_rook(const Square sq) const {
  Square pinned{}; // potentially pinned piece
                   // xray through ONE enemy piece
  const Color c = what_color(sq);

  // vertical up
  bool found_one = false;
  for (auto square = sq + 8; !is_upper_vertical_boundary(square - 8);
       square = square + 8) {
    if (!is_empty(square) && is_same_color(square, c)) {
      break;
    }
    if (found_one) {
      // after the first non-king opponent has been found, if we find another,
      // end searching in this direction
      if (!is_empty(square) && !is_same_color(square, c) &&
          !is_opposite_king(square, c)) {
        break;
      }
      // after the first ... if we find the opposing king, that first piece is
      // pinned.  YAY
      if (is_opposite_king(square, c)) {
        return pinned;
      }
    }
    // if it is occupied by an opponent that is not a king
    else if (!is_empty(square) && !is_same_color(square, c) &&
             !is_opposite_king(square, c)) {
      pinned = square; // possible pinned piece
      found_one = true;
    }
  }
  // vertical down
  found_one = false;
  for (auto square = sq - 8; !is_lower_vertical_boundary(square + 8);
       square = square - 8) {
    if (!is_empty(square) && is_same_color(square, c)) {
      break;
    }
    if (found_one) {
      // after the first non-king opponent has been found, if we find another,
      // end searching in this direction
      if (!is_empty(square) && !is_same_color(square, c) &&
          !is_opposite_king(square, c)) {
        break;
      }
      // after the first ... if we find the opposing king, that first piece is
      // pinned.  YAY
      if (is_opposite_king(square, c)) {
        return pinned;
      }
    }
    // if it is occupied by an opponent that is not a king
    else if (!is_empty(square) && !is_same_color(square, c) &&
             !is_opposite_king(square, c)) {
      pinned = square; // possible pinned piece
      found_one = true;
    }
  }
  // horizontal right
  found_one = false;
  for (auto square = sq - 1; !is_right_horizontal_boundary(square + 1);
       square = square - 1) {
    if (!is_empty(square) && is_same_color(square, c)) {
      break;
    }
    if (found_one) {
      // after the first non-king opponent has been found, if we find another,
      // end searching in this direction
      if (!is_empty(square) && !is_same_color(square, c) &&
          !is_opposite_king(square, c)) {
        break;
      }
      // after the first ... if we find the opposing king, that first piece is
      // pinned.  YAY
      if (is_opposite_king(square, c)) {
        return pinned;
      }
    }
    // if it is occupied by an opponent that is not a king
    else if (!is_empty(square) && !is_same_color(square, c) &&
             !is_opposite_king(square, c)) {
      pinned = square; // possible pinned piece
      found_one = true;
    }
  }
  // horizontal left
  found_one = false;
  for (auto square = sq + 1; !is_left_horizontal_boundary(square - 1);
       square = square + 1) {
    if (!is_empty(square) && is_same_color(square, c)) {
      break;
    }
    if (found_one) {
      // after the first non-king opponent has been found, if we find another,
      // end searching in this direction
      if (!is_empty(square) && !is_same_color(square, c) &&
          !is_opposite_king(square, c)) {
        break;
      }
      // after the first ... if we find the opposing king, that first piece is
      // pinned.  YAY
      if (is_opposite_king(square, c)) {
        return pinned;
      }
    }
    // if it is occupied by an opponent that is not a king
    else if (!is_empty(square) && !is_same_color(square, c) &&
             !is_opposite_king(square, c)) {
      pinned = square; // possible pinned piece
      found_one = true;
    }
  }
  return sq;
}

// END pinned pieces rook
//------------------------------------------------------------------------------
// BEGIN pinned piece bishop

Square Board::pinned_piece_bishop(const Square sq) const {
  Square pinned{}; // potentially pinned piece
                   // xray through ONE enemy piece
  const Color c = what_color(sq);

  // up left
  bool found_one = false;
  for (auto square = sq + 9; !is_upper_vertical_boundary(square - 9) &&
                             !is_left_horizontal_boundary(square - 9);
       square = square + 9) {
    if (!is_empty(square) && is_same_color(square, c)) {
      break;
    }
    if (found_one) {
      // after the first non-king opponent has been found, if we find another,
      // end searching in this direction
      if (!is_empty(square) && !is_same_color(square, c) &&
          !is_opposite_king(square, c)) {
        break;
      }
      // after the first ... if we find the opposing king, that first piece is
      // pinned.  YAY
      if (is_opposite_king(square, c)) {
        return pinned;
      }
    }
    // if it is occupied by an opponent that is not a king
    else if (!is_empty(square) && !is_same_color(square, c) &&
             !is_opposite_king(square, c)) {
      pinned = square; // possible pinned piece
      found_one = true;
    }
  }
  // up right
  found_one = false;
  for (auto square = sq + 7; !is_upper_vertical_boundary(square - 7) &&
                             !is_right_horizontal_boundary(square - 7);
       square = square + 7) {
    if (!is_empty(square) && is_same_color(square, c)) {
      break;
    }
    if (found_one) {
      // after the first non-king opponent has been found, if we find another,
      // end searching in this direction
      if (!is_empty(square) && !is_same_color(square, c) &&
          !is_opposite_king(square, c)) {
        break;
      }
      // after the first ... if we find the opposing king, that first piece is
      // pinned.  YAY
      if (is_opposite_king(square, c)) {
        return pinned;
      }
    }
    // if it is occupied by an opponent that is not a king
    else if (!is_empty(square) && !is_same_color(square, c) &&
             !is_opposite_king(square, c)) {
      pinned = square; // possible pinned piece
      found_one = true;
    }
  }
  // down right
  found_one = false;
  for (auto square = sq - 9; !is_lower_vertical_boundary(square + 9) &&
                             !is_right_horizontal_boundary(square + 9);
       square = square - 9) {
    if (!is_empty(square) && is_same_color(square, c)) {
      break;
    }
    if (found_one) {
      // after the first non-king opponent has been found, if we find another,
      // end searching in this direction
      if (!is_empty(square) && !is_same_color(square, c) &&
          !is_opposite_king(square, c)) {
        break;
      }
      // after the first ... if we find the opposing king, that first piece is
      // pinned.  YAY
      if (is_opposite_king(square, c)) {
        return pinned;
      }
    }
    // if it is occupied by an opponent that is not a king
    else if (!is_empty(square) && !is_same_color(square, c) &&
             !is_opposite_king(square, c)) {
      pinned = square; // possible pinned piece
      found_one = true;
    }
  }
  // down left
  found_one = false;
  for (auto square = sq - 7; !is_lower_vertical_boundary(square + 7) &&
                             !is_left_horizontal_boundary(square + 7);
       square = square - 7) {
    if (!is_empty(square) && is_same_color(square, c)) {
      break;
    }
    if (found_one) {
      // after the first non-king opponent has been found, if we find another,
      // end searching in this direction
      if (!is_empty(square) && !is_same_color(square, c) &&
          !is_opposite_king(square, c)) {
        break;
      }
      // after the first ... if we find the opposing king, that first piece is
      // pinned.  YAY
      if (is_opposite_king(square, c)) {
        return pinned;
      }
    }
    // if it is occupied by an opponent that is not a king
    else if (!is_empty(square) && !is_same_color(square, c) &&
             !is_opposite_king(square, c)) {
      pinned = square; // possible pinned piece
      found_one = true;
    }
  }
  return sq;
}

// END pinned piece bishop
//------------------------------------------------------------------------------
// BEGIN pinned piece queen

Square Board::pinned_piece_queen(const Square sq) const {
  if (pinned_piece_rook(sq) != sq) {
    return pinned_piece_rook(sq);
  }
  if (pinned_piece_bishop(sq) != sq) {
    return pinned_piece_bishop(sq);
  }
  return sq;
}

// END pinned piece queen
//------------------------------------------------------------------------------
// BEGIN pinned pieces

Square Board::pinned_piece(const Square sq) const {
  if (is_rook(sq)) {
    if (pinned_piece_rook(sq) != sq) {
      return pinned_piece_rook(sq);
    }
  }
  if (is_bishop(sq)) {
    if (pinned_piece_bishop(sq) != sq) {
      return pinned_piece_bishop(sq);
    }
  }
  if (is_queen(sq)) {
    if (pinned_piece_queen(sq) != sq) {
      return pinned_piece_queen(sq);
    }
  }

  return sq;
}

// END pinned pieces
//------------------------------------------------------------------------------
// BEGIN update pinned pieces

void Board::update_pinned_pieces() const {
  // reset the maps
  maps->white_pinned.clear();
  maps->black_pinned.clear();

  for (const auto &[sq, moves] : maps->white_influence) {
    if (pinned_piece(sq) != sq) {
      maps->black_pinned.insert({pinned_piece(sq), sq});
    }
  }
  for (const auto &[sq, moves] : maps->black_influence) {
    if (pinned_piece(sq) != sq) {
      maps->white_pinned.insert({pinned_piece(sq), sq});
    }
  }
}

// END update pinned pieces
//------------------------------------------------------------------------------
// BEGIN update king moves

// TODO test with ranges rewrite
std::vector<Square> Board::update_white_king_moves(Square sq_w_King) {
  std::vector<Square> giving_check{};

  std::vector<Square> safe_squares;      // temp vector to create king move list
  std::vector<Square> legal_moves_kings; // temp vector to create king move list

  // add castle squares if castling is still valid. Must be done before cleaning
  // the list because while castling ability may be present, is may not always
  // be legal to do so, because enemy pieces could be attacking those squares,
  // or friendly pieces may be blocking them. piece giving check

  // temp vector to create king move list
  std::vector<Square> influence_kings = influence(sq_w_King);

  // detect whether king is in check
  bool is_in_check = false; // king is in check
  for (const auto &[fst, snd] : maps->black_influence) {
    if (std::ranges::find(snd, sq_w_King) !=
        snd.end()) { // is the king in check
      is_in_check = true;
      game_state.white_inCheck = true;
      // keep track of who is giving check
      giving_check.push_back(fst);
    }
  }
  if (!is_in_check) { // if not in check, add castling moves (if available)
    if (game_state.castle_w_K) {
      influence_kings.push_back(s::g1);
    }
    if (game_state.castle_w_Q) {
      influence_kings.push_back(s::c1);
    }
  }

  // remove square that are under attack
  for (const auto &s : influence_kings) {
    // flags a square as under attack
    bool under_attack = false; // reset search flag
    for (const auto &[fst, snd] : maps->black_influence) {
      if (std::ranges::find(snd, s) != snd.end()) {
        under_attack = true;
        break;
      }
    }
    if (!under_attack) {
      safe_squares.push_back(s);
    }
  }
  // clean list of same color squares (cannot take own pieces)
  for (const auto &s : safe_squares) {
    if (!is_same_color(s, c::white)) {
      legal_moves_kings.push_back(s);
    }
  }

  // if the legal moves contain a castling square, but not the adjacent square
  // necessary to legally castle remove the castling square
  if (game_state.castle_w_K) {
    if (std::ranges::find(legal_moves_kings, s::g1) !=
        legal_moves_kings.end()) {
      if (std::ranges::find(legal_moves_kings, s::f1) ==
          legal_moves_kings.end()) {
        // remove g1
        const auto it = std::ranges::remove(legal_moves_kings, s::g1).begin();
        legal_moves_kings.erase(it, legal_moves_kings.end());
      }
    }
  }
  if (game_state.castle_w_Q) {
    if (std::ranges::find(legal_moves_kings, s::c1) !=
        legal_moves_kings.end()) {
      if (std::ranges::find(legal_moves_kings, s::d1) ==
          legal_moves_kings.end()) {
        // remove c1
        const auto it = std::ranges::remove(legal_moves_kings, s::c1).begin();
        legal_moves_kings.erase(it, legal_moves_kings.end());
      }
      // when castling long, gotta check the third square too. if b1 isn't
      // empty, can't castle just yet
      else if (!is_empty(s::b1)) {
        const auto it = std::ranges::remove(legal_moves_kings, s::c1).begin();
        legal_moves_kings.erase(it, legal_moves_kings.end());
      }
    }
  }

  // add king moves to move map
  maps->white_moves.insert({sq_w_King, legal_moves_kings});

  return giving_check;
}

// TODO test with ranges rewrite
std::vector<Square> Board::update_black_king_moves(Square sq_b_king) {
  std::vector<Square> giving_check{};
  std::vector<Square> safe_squares;      // temp vector to create king move list
  std::vector<Square> legal_moves_kings; // temp vector to create king move list

  // temp vector to create king move list
  std::vector<Square> influence_kings = influence(sq_b_king);

  // detect whether king is in check
  bool is_inCheck = false;
  for (const auto &[fst, snd] : maps->white_influence) {
    if (std::ranges::find(snd, sq_b_king) !=
        snd.end()) { // is the king in check
      is_inCheck = true;
      game_state.black_inCheck = true;
      giving_check.push_back(fst);
    }
  }
  if (!is_inCheck) { // if not in check, add castling moves (if available)
    if (game_state.castle_b_k) {
      influence_kings.push_back(s::g8);
    }
    if (game_state.castle_b_q) {
      influence_kings.push_back(s::c8);
    }
  }

  // remove square that are under attack
  for (const auto &s : influence_kings) {
    bool under_attack = false; // reset search flag
    for (const auto &[fst, snd] : maps->white_influence) {
      if (std::ranges::find(snd, s) != snd.end()) {
        under_attack = true;
        break;
      }
    }
    if (!under_attack) {
      safe_squares.push_back(s);
    }
  }
  // clean list of same color squares (cannot take own pieces)
  for (const auto &s : safe_squares) {
    if (!is_same_color(s, c::black)) {
      legal_moves_kings.push_back(s);
    }
  }

  // if the legal moves contain a castling square, but not the adjacent square
  // necessary to legally castle remove the castling square
  if (game_state.castle_b_k) {
    if (std::ranges::find(legal_moves_kings, s::g8) !=
        legal_moves_kings.end()) {
      if (std::ranges::find(legal_moves_kings, s::f8) ==
          legal_moves_kings.end()) {
        // remove g8
        const auto it = std::ranges::remove(legal_moves_kings, s::g8).begin();
        legal_moves_kings.erase(it, legal_moves_kings.end());
      }
    }
  }
  if (game_state.castle_b_q) {
    if (std::ranges::find(legal_moves_kings, s::c8) !=
        legal_moves_kings.end()) {
      if (std::ranges::find(legal_moves_kings, s::d8) ==
          legal_moves_kings.end()) {
        // remove c1
        const auto it = std::ranges::remove(legal_moves_kings, s::c8).begin();
        legal_moves_kings.erase(it, legal_moves_kings.end());
      }
      // when castling long, gotta check the third square too. if b8 isn't
      // empty, can't castle just yet
      else if (!is_empty(s::b8)) {
        const auto it = std::ranges::remove(legal_moves_kings, s::c8).begin();
        legal_moves_kings.erase(it, legal_moves_kings.end());
      }
    }
  }

  // add king moves to move map
  maps->black_moves.insert({sq_b_king, legal_moves_kings});

  return giving_check;
}
// END update king moves
//------------------------------------------------------------------------------
// BEGIN update move maps

Square Board::copy_influence(
    const Color c, const std::unordered_map<Square, std::vector<Square>> *from,
    std::unordered_map<Square, std::vector<Square>> *to) const {
  Square sq_king{};
  to->clear();
  if (c == c::white) {
    for (const auto &[fst, snd] : *from) {
      if (!is_white_pawn(fst) && !is_white_king(fst)) {
        to->insert({fst, snd});
      } else if (is_white_king(fst)) {
        sq_king = fst;
      }
    }
  } else if (c == c::black) {
    for (const auto &[fst, snd] : *from) {
      if (!is_black_pawn(fst) && !is_black_king(fst)) {
        to->insert({fst, snd});
      } else if (is_black_king(fst)) {
        sq_king = fst;
      }
    }
  }
  return sq_king;
}

void Board::remove_same_color_squares(
    const Color color,
    std::unordered_map<Square, std::vector<Square>> *map) const {
  std::unordered_map<Square, std::vector<Square>>
      temp;                           // to reassign original after cleaning
  std::vector<Square> temp_squares{}; // to store non-same-colored squares
  temp_squares.reserve(16);

  for (const auto &[fst, snd] : *map) {
    temp_squares.clear(); // reset temp vector
    // record every non-same-colored square
    for (const auto &sq : snd) {
      if (!is_same_color(sq, color)) {
        temp_squares.push_back(sq);
      }
    }
    temp.insert({fst, temp_squares}); // insert into "scrubbed" map
  }
  *map = temp; // reassign original map
}

void Board::update_move_maps() {
  /**
   * @brief Clear all the maps in a Maps object.
   */
  maps->clear();                    // clear move maps
  game_state.white_inCheck = false; // reset check flags
  game_state.black_inCheck = false;
  update_influence_maps(); // update influence maps

  // copy influence maps, exclude pawns and kings; we have a special treatment
  // for them
  const Square sq_w_King =
      copy_influence(Color::white, &maps->white_influence, &maps->white_moves);
  const Square sq_b_king =
      copy_influence(Color::black, &maps->black_influence, &maps->black_moves);

  update_pinned_pieces(); // update pinned pieces

  // start reduction ------------------------------------
  remove_same_color_squares(Color::white,
                            &maps->white_moves); // remove same colored squares
  remove_same_color_squares(Color::black,
                            &maps->black_moves); // remove same colored squares

  for (const auto &[sq, moves] : maps->white_influence) {
    if (is_white_pawn(sq)) {
      maps->white_moves.insert({sq, legal_moves(sq)});
    } // generate white pawn moves
  }
  for (const auto &[sq, moves] : maps->black_influence) {
    if (is_black_pawn(sq)) {
      maps->black_moves.insert({sq, legal_moves(sq)});
    } // generate black pawn moves
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
  // and there is only one friendly piece sharing that line - that piece is
  // pinned the pieces movement must be restricted to the line of the pin there
  // may be multiple lines of pinning

  // find all the rows, columns, and diagonals that the king shares with
  // opposite colored rooks, queens, and bishops

  // if one piece is giving check, it can be captured or blocked
  if (giving_check.size() == 1) {
    // if it's not a knight or pawn, it might be able to be blocked
    if (!is_knight(giving_check[0]) && !is_pawn(giving_check[0])) {
      if (is_inSameRow(giving_check[0], sq_w_King)) {
        for (auto sq = sq_w_King; sq != giving_check[0];
             sq_w_King > giving_check[0] ? --sq : ++sq) {
          blocking_squares.push_back(sq);
        }
      } else if (is_inSameColumn(giving_check[0], sq_w_King)) {
        for (auto sq = sq_w_King; sq != giving_check[0];
             sq_w_King > giving_check[0] ? sq = sq - 8 : sq = sq + 8) {
          blocking_squares.push_back(sq);
        }
      } else if (is_inSameDiagonal_leftRight(giving_check[0], sq_w_King)) {
        for (auto sq = sq_w_King; sq != giving_check[0];
             sq_w_King > giving_check[0] ? sq = sq - 7 : sq = sq + 7) {
          blocking_squares.push_back(sq);
        }
      } else if (is_inSameDiagonal_rightLeft(giving_check[0], sq_w_King)) {
        for (auto sq = sq_w_King; sq != giving_check[0];
             sq_w_King > giving_check[0] ? sq = sq - 9 : sq = sq + 9) {
          blocking_squares.push_back(sq);
        }
      }
    }
    for (auto &[fst, snd] : maps->white_moves) {
      if (!is_king(fst)) {
        std::vector<Square> temp{};
        for (const auto &sq : snd) {
          for (const auto &block_sq : blocking_squares) {
            if (sq == block_sq) {
              temp.push_back(sq);
            }
          }
          if (sq == giving_check[0]) {
            temp.push_back(sq);
          }
        }
        snd = temp;
      }
    }
  }
  // if more than once piece is giving check, the king must be moved
  else if (giving_check.size() > 1) {
    // clear all moves except king evasion
    for (auto &[fst, snd] : maps->white_moves) {
      if (!is_king(fst)) {
        snd.clear();
      }
    }
  }

  // black
  giving_check.clear();
  giving_check = update_black_king_moves(sq_b_king);
  blocking_squares.clear(); // reset
  // if one piece is giving check, it can be captured or blocked
  if (giving_check.size() == 1) {
    if (!is_knight(giving_check[0]) && !is_pawn(giving_check[0])) {
      if (is_inSameRow(giving_check[0], sq_b_king)) {
        for (auto sq = sq_b_king; sq != giving_check[0];
             sq_b_king > giving_check[0] ? --sq : ++sq) {
          blocking_squares.push_back(sq);
        }
      } else if (is_inSameColumn(giving_check[0], sq_b_king)) {
        for (auto sq = sq_b_king; sq != giving_check[0];
             sq_b_king > giving_check[0] ? sq = sq - 8 : sq = sq + 8) {
          blocking_squares.push_back(sq);
        }
      } else if (is_inSameDiagonal_leftRight(giving_check[0], sq_b_king)) {
        for (auto sq = sq_b_king; sq != giving_check[0];
             sq_b_king > giving_check[0] ? sq = sq - 7 : sq = sq + 7) {
          blocking_squares.push_back(sq);
        }
      } else if (is_inSameDiagonal_rightLeft(giving_check[0], sq_b_king)) {
        for (auto sq = sq_b_king; sq != giving_check[0];
             sq_b_king > giving_check[0] ? sq = sq - 9 : sq = sq + 9) {
          blocking_squares.push_back(sq);
        }
      }
    }
    for (auto &[fst, snd] : maps->black_moves) {
      if (!is_king(fst)) {
        std::vector<Square> temp{};
        for (const auto &sq : snd) {
          for (const auto &block_sq : blocking_squares) {
            if (sq == block_sq) {
              temp.push_back(sq);
            }
          }
          if (sq == giving_check[0]) {
            temp.push_back(sq);
          }
        }
        snd = temp;
      }
    }
  }
  // if more than once piece is giving check, the king must be moved
  else if (giving_check.size() > 1) {
    // clear all moves except king evasion
    for (auto &[fst, snd] : maps->black_moves) {
      if (!is_king(fst)) {
        snd.clear();
      }
    }
  }

  // pinned piece reduction
  // white
  for (const auto &[pinned, pinner] : maps->white_pinned) {
    for (auto &[piece, moves] : maps->white_moves) {
      if (piece == pinned) {
        std::vector<Square> temp{};
        for (auto &sq : moves) {
          if (is_inSameRow(sq_w_King, pinner, sq)) {
            temp.push_back(sq);
          }
          if (is_inSameColumn(sq_w_King, pinner, sq)) {
            temp.push_back(sq);
          }
          if (is_inSameDiagonal_leftRight(sq_w_King, pinner, sq)) {
            temp.push_back(sq);
          }
          if (is_inSameDiagonal_rightLeft(sq_w_King, pinner, sq)) {
            temp.push_back(sq);
          }
        }
        moves = temp;
      }
    }
  }
  // black
  for (const auto &[pinned, pinner] : maps->black_pinned) {
    for (auto &[piece, moves] : maps->black_moves) {
      if (piece == pinned) {
        std::vector<Square> temp{};
        for (auto &sq : moves) {
          if (is_inSameRow(sq_b_king, pinner, sq)) {
            temp.push_back(sq);
          }
          if (is_inSameColumn(sq_b_king, pinner, sq)) {
            temp.push_back(sq);
          }
          if (is_inSameDiagonal_leftRight(sq_b_king, pinner, sq)) {
            temp.push_back(sq);
          }
          if (is_inSameDiagonal_rightLeft(sq_b_king, pinner, sq)) {
            temp.push_back(sq);
          }
        }
        moves = temp;
      }
    }
  }
}

// END update move maps
//------------------------------------------------------------------------------
// BEGIN move

// TODO test move pawn
void Board::move_pawn(Square from, Square to, const char ch) {
  // promotion
  if (ch == 'q' || ch == 'r' || ch == 'b' || ch == 'n') {
    const Color c = what_color(from);
    if ((is_white_pawn(from) && get_row(to) == 8) ||
        (is_black_pawn(from) && get_row(to) == 1)) {
      remove_piece(to);
      place_piece(to,
                  c == Color::white ? static_cast<char>(std::toupper(ch)) : ch);
      remove_piece(from);
    }
  }
  // update en passant square
  // comes first because 'from' will be emptied after the move
  else {
    if (is_white_pawn(from)) {
      if (static_cast<int>(to) - static_cast<int>(from) == 16) {
        game_state.en_passant_target = Sq::square_to_string(from + 8);
        game_state.en_passant_set = true;
      } else if (Sq::square_to_string(to) == game_state.en_passant_target) {
        remove_piece(Sq::string_to_square(game_state.en_passant_target) - 8);
      }
    } else if (is_black_pawn(from)) {
      if (static_cast<int>(from) - static_cast<int>(to) == 16) {
        game_state.en_passant_target = Sq::square_to_string(from - 8);
        game_state.en_passant_set = true;
      } else if (Sq::square_to_string(to) == game_state.en_passant_target) {
        remove_piece(Sq::string_to_square(game_state.en_passant_target) + 8);
      }
    }

    // move piece
    remove_piece(to);
    place_piece(to, what_piece(from));
    remove_piece(from);
  }
}

void Board::move_king(const Square from, const Square to) {
  bool castled = false;

  // check for castling
  if (is_white_king(from) && from == s::e1) {
    if (game_state.castle_w_K && to == s::g1 && is_white_rook(s::h1)) {
      remove_piece(to);
      place_piece(to, what_piece(from));
      remove_piece(from);
      move_rook(s::h1, s::f1);
      castled = true;
      game_state.castle_w_K = game_state.castle_w_Q = false;
    } else if (game_state.castle_w_Q && to == s::c1 && is_white_rook(s::a1)) {
      remove_piece(to);
      place_piece(to, what_piece(from));
      remove_piece(from);
      move_rook(s::a1, s::d1);
      castled = true;
      game_state.castle_w_K = game_state.castle_w_Q = false;
    }
  }
  if (is_black_king(from) && from == s::e8) {
    if (game_state.castle_b_k && to == s::g8 && is_black_rook(s::h8)) {
      remove_piece(to);
      place_piece(to, what_piece(from));
      remove_piece(from);
      move_rook(s::h8, s::f8);
      castled = true;
      game_state.castle_b_k = game_state.castle_b_q = false;
    } else if (game_state.castle_b_q && to == s::c8 && is_black_rook(s::a8)) {
      remove_piece(to);
      place_piece(to, what_piece(from));
      remove_piece(from);
      move_rook(s::a8, s::d8);
      castled = true;
      game_state.castle_b_k = game_state.castle_b_q = false;
    }
  }

  // update castling rights
  if (is_white_king(from)) {
    game_state.castle_w_K = game_state.castle_w_Q = false;
  } else if (is_black_king(from)) {
    game_state.castle_b_k = game_state.castle_b_q = false;
  }

  // move piece
  if (!castled) {
    remove_piece(to);
    place_piece(to, what_piece(from));
    remove_piece(from);
  }
}

void Board::move_rook(const Square from, const Square to) {
  // update castling rights
  if (game_state.castle_b_q && from == s::a8 && is_black_rook(from)) {
    game_state.castle_b_q = false;
  } else if (game_state.castle_b_k && from == s::h8 && is_black_rook(from)) {
    game_state.castle_b_k = false;
  } else if (game_state.castle_w_Q && from == s::a1 && is_white_rook(from)) {
    game_state.castle_w_Q = false;
  } else if (game_state.castle_w_K && from == s::h1 && is_white_rook(from)) {
    game_state.castle_w_K = false;
  }

  // move piece
  remove_piece(to);
  place_piece(to, what_piece(from));
  remove_piece(from);
}

void Board::move_piece(const Square from, const Square to) {
  remove_piece(to);
  place_piece(to, what_piece(from));
  remove_piece(from);
}

void Board::do_move(const Square from, const Square to, const char ch) {
  // game state updates
  // comes first because from and to will change occupants after the move
  !game_state.active_color; // swap active color
  if (is_black(from)) {
    game_state.full_move_number++;
  } // increment full move every time black moves

  // 50 move rule: draw if no pawn move or capture for 50 moves
  if (is_pawn(from) ||
      (what_color(to) != Color::none && !is_same_color(from, what_color(to)))) {
    game_state.half_move_clock = 0;
  } else {
    game_state.half_move_clock++;
  }

  // update castling rights if capturing rook
  if (is_white_rook(to)) {
    if (to == s::h1) {
      game_state.castle_w_K = false;
    } else if (to == s::a1) {
      game_state.castle_w_Q = false;
    }
  } else if (is_black_rook(to)) {
    if (to == s::h8) {
      game_state.castle_b_k = false;
    } else if (to == s::a8) {
      game_state.castle_b_q = false;
    }
  }

  // move piece
  if (is_pawn(from)) {
    move_pawn(from, to, ch);
  } else if (is_king(from)) {
    move_king(from, to);
  } else if (is_rook(from)) {
    move_rook(from, to);
  } else if (!is_empty(from)) {
    move_piece(from, to);
  } // for knights, bishops, queens

  // en passant expires
  if (!game_state.en_passant_set && !game_state.en_passant_target.empty()) {
    game_state.en_passant_target.clear();
  }
  if (game_state.en_passant_set) {
    game_state.en_passant_set = false;
  }
}

// END move
//------------------------------------------------------------------------------
// BEGIN diagnostic

ulong Board::nodes_at_depth_1(const Color color) {
  update_move_maps();
  ulong nodes{};
  for (const auto &[sq, moves] :
       color == Color::white ? maps->white_moves : maps->black_moves) {
    nodes += moves.size();
  }
  return nodes;
}
