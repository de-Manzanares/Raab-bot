module;

import board;
import defs;

module attack;

namespace raab_bot {

bool ia_pawn(const Board &b, Square sq, Color by_color);
bool ia_non_sliding(const Board &b, Square sq, Color by_color, Piece piece);
bool ia_sliding(const Board &b, Square sq, Color by_color, Piece piece);

//------------------------------------------------------------------------------

bool is_attacked(const Board &b, const Square sq, const Color by_color)
{
  if (ia_pawn(b, sq, by_color)) {
    return true;
  }
  if (ia_sliding(b, sq, by_color, bishop) || ia_sliding(b, sq, by_color, rook)) {
    return true;
  }
  if (ia_non_sliding(b, sq, by_color, knight) || ia_non_sliding(b, sq, by_color, king)) {
    return true;
  }
  return false;
}

bool in_check(const Board &b)
{
  const Square sq = b.stm == white ? b.wks : b.bks;
  return is_attacked(b, sq, ~b.stm);
}

//------------------------------------------------------------------------------

/// attacked by a pawn ?
bool ia_pawn(const Board &b, const Square sq, const Color by_color)
{
  for (constexpr Direction pawn_atk_vec[2][2] =
           {
               {SW, SE},
               {NW, NE}
  };
       const auto dir : pawn_atk_vec[by_color]) {
    if (const Square vec{sq + dir};
        is_on_board(vec) && b.piece_info(vec) == PieceInfo{pawn, by_color}) {
      return true;
    }
  }
  return false;
}

/// attacked by a knight || king ?
bool ia_non_sliding(const Board &b, const Square sq, const Color by_color, const Piece piece)
{
  for (const auto dir : unit_vectors[piece]) {
    if (const Square vec{sq + dir};
        is_on_board(vec) && b.piece_info(vec) == PieceInfo{piece, by_color}) {
      return true;
    }
  }
  return false;
}

/// attacked by a <bishop || rook> || queen ?
/// @warning needs to be called twice - once for bishop and rook each
bool ia_sliding(const Board &b, const Square sq, const Color by_color, const Piece piece)
{
  sz_t directions = 4;
  for (const auto dir : unit_vectors[piece]) {
    for (int magnitude = 1;; ++magnitude) {
      const Square vec{sq + dir * magnitude};
      if (!is_on_board(vec)) {
        break;
      }
      const auto pi = b.piece_info(vec);
      if (pi.piece_t == null_piece) { // empty square
        continue;
      }
      if (pi.color == ~by_color) { // friendly occupied square
        break;
      }
      if (pi == PieceInfo{piece, by_color} || pi == PieceInfo{queen, by_color}) {
        return true;
      }
      if (pi.piece_t != null_piece) {
        break;
      }
    }
    if (--directions == 0) {
      break;
    }
  }
  return false;
}

} // namespace raab_bot
