/**
 * @file Board0x88.cppm
 * interface and implementation of the 0x88 board representation
 */

module;

#include <array>
#include <iostream>
#include <string_view>

export module Board0x88:core;
import :fen;
import :types;

//------------------------------------------------------------------------------

/**
 * @class Board
 * @brief 0x88 board representation and associated functionality
 */
export class Board {
 public:
  Board() : Board(fen::startpos) {}     ///< default startpos
  explicit Board(std::string_view fen); ///< set up the board with a fen string

  [[nodiscard]] PieceInfo piece_info(Square sq) const;

  void display() const; ///< print a simple visualization of the board

  // piece tracking

  std::array<Piece, 128> piece_on{}; ///< 0x88 board representation
  std::array<Color, 128> color_on{}; ///< redundant 0x88 color tracking

  // game state tracking

  Color stm;                    ///< side to move
  Square ep;                    ///< en passant square
  int castling_rights = 0b0000; ///< bqs = 8, bks = 4, wqs = 2, wks = 1
  int hmc;                      ///< half move clock
  int fmc;                      ///< full move clock

  int wks{}; ///< white king square
  int bks{}; ///< black king square
};

/**
 * @details The classic 0x88 square checking trick :-)
 * @param sq the square in question
 * @return true - is on board, false - is off board
 */
export bool is_valid_square(const Square sq) { return (sq & 0x88) == 0; }

//------------------------------------------------------------------------------

Board::Board(const std::string_view fen) {
  using namespace fen;

  piece_on.fill(null_piece);
  color_on.fill(null_color);

  int i = 0;
  auto ch = fen.begin();

  for (; i < 64; ++ch) {
    if (is_piece(*ch)) {
      const auto [piece_type, color] = get_piece_info(*ch);
      const auto sq = to_0x88_idx(i);
      piece_on[sq] = piece_type;
      color_on[sq] = color;
      if (piece_type == king) {
        if (color == white) {
          wks = sq;
        } else if (color == black) {
          bks = sq;
        }
      }
      i++;
    } else if (*ch >= '1' && *ch <= '8') { // empty squares
      i += *ch - '0';
    } else if (*ch == ' ') {
      break;
    }
  }

  // minimal error checking
  // did we get all the squares?
  if (i != 64) {
    throw std::runtime_error("malformed FEN string");
  }

  // side to move
  ++ch;
  *ch == 'w' ? stm = white : stm = black;
  std::advance(ch, 2);

  // castling rights
  if (*ch == '-') {
    castling_rights = 0;
    ++ch;
  } else {
    for (; ch != fen.end() && *ch != ' '; ++ch) {
      switch (*ch) {
      case 'K':
        castling_rights += 0b0001;
        break;
      case 'Q':
        castling_rights += 0b0010;
        break;
      case 'k':
        castling_rights += 0b0100;
        break;
      case 'q':
        castling_rights += 0b1000;
        break;
      default:
      }
    }
  }

  // en passant target
  ++ch;
  if (*ch == '-') {
    ep = null_square;
    std::advance(ch, 2);
  } else {
    ep = static_cast<Square>((16 * (*std::next(ch) - '0' - 1)) + *ch - 'a');
    std::advance(ch, 3);
  }

  if (ch == fen.end() || std::next(ch) == fen.end()) {
    return;
  }

  // half-move clock, full-move clock
  hmc = *ch - '0';
  fmc = *std::next(ch, 2) - '0';
}

PieceInfo Board::piece_info(const Square sq) const {
  return {.piece_type = piece_on[sq], .color = color_on[sq]};
}

void Board::display() const {
  using namespace fen;

  for (int i = 0; i < 64; ++i) {
    const auto sq = to_0x88_idx(i);
    const PieceInfo pi{.piece_type = piece_on[sq], .color = color_on[sq]};
    const char ch = get_char_code(pi);
    std::cout << ' ' << ch;
    if ((i + 1) % 8 == 0) {
      std::cout << '\n';
    }
  }
}
