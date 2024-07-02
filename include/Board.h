/*
 *     ____              __          __          __
 *    / __ \____ _____ _/ /_        / /_  ____  / /_
 *   / /_/ / __ `/ __ `/ __ \______/ __ \/ __ \/ __/
 *  / _, _/ /_/ / /_/ / /_/ /_____/ /_/ / /_/ / /_
 * /_/ |_|\__,_/\__,_/_.___/     /_.___/\____/\__/
 *
 * Copyright (c) 2024 de-Manzanares
 * This work is released under the MIT license.
 *
 */

#ifndef INCLUDE_BOARD_H_
#define INCLUDE_BOARD_H_

#include "Game_State.h"
#include "Square.h"

#include <cstdint>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @struct Diagonals
 * @brief Groups the diagonals of the board for answering the question "are
 * these squares in the same diagonal?"
 */
struct Diagonals {
  static std::vector<std::vector<Square>>
      leftRight; ///< left to right {diagonal,{squares}}
  static std::vector<std::vector<Square>>
      rightLeft; ///< right to left {diagonal,{squares}}
};

/**
 * @class Maps
 * @brief A collection of unordered maps used for move generation
 */
struct Maps {
  // influence - possible moves, squares under attack/defense by this piece
  std::unordered_map<Square, std::vector<Square>>
      white_influence{}; ///< all white influence
  std::unordered_map<Square, std::vector<Square>>
      black_influence{}; ///< all black influence
  std::unordered_map<Square, Square>
      white_pinned{}; ///< {pinned piece, pinning piece}
  std::unordered_map<Square, Square>
      black_pinned{}; ///< {pinned piece, pinning piece}
  std::unordered_map<Square, std::vector<Square>>
      white_moves{}; ///< all legal white moves
  std::unordered_map<Square, std::vector<Square>>
      black_moves{}; ///< all legal black moves
  void clear();
};

/**
 * @struct Board
 * @brief Represents the chessboard
 * @details Stores all relevant game-state data and enforces rules during move
 * generation.
 * @note Bitboards are initialized with standard startpos.
 */
struct Board {
  ~Board();
  /**
   * @brief Copy assignment operator
   * @param rhs The Board object to be copied
   * @return A reference to the lhs object after assignment
   */
  Board &operator=(const Board &rhs);
  // clang-format off
  uint64_t b_pawn   = 0b00000000'11111111'00000000'00000000'00000000'00000000'00000000'00000000;    ///< black pawn
  uint64_t b_night  = 0b01000010'00000000'00000000'00000000'00000000'00000000'00000000'00000000;    ///< black knight
  uint64_t b_bishop = 0b00100100'00000000'00000000'00000000'00000000'00000000'00000000'00000000;    ///< black bishop
  uint64_t b_rook   = 0b10000001'00000000'00000000'00000000'00000000'00000000'00000000'00000000;    ///< black rook
  uint64_t b_queen  = 0b00010000'00000000'00000000'00000000'00000000'00000000'00000000'00000000;    ///< black queen
  uint64_t b_king   = 0b00001000'00000000'00000000'00000000'00000000'00000000'00000000'00000000;    ///< black king
  uint64_t w_Pawn   = 0b00000000'00000000'00000000'00000000'00000000'00000000'11111111'00000000;    ///< white pawn
  uint64_t w_Night  = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'01000010;    ///< white knight
  uint64_t w_Bishop = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00100100;    ///< white bishop
  uint64_t w_Rook   = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'10000001;    ///< white rook
  uint64_t w_Queen  = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00010000;    ///< white queen
  uint64_t w_King   = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00001000;    ///< white king
  // clang-format on

  Game_State game_state; ///< game conditions aside from piece placement

  Maps *maps = new Maps; ///< for move generation

  /// Maps character code to the respective bitboard.
  std::unordered_map<char, uint64_t &> piece_map = {
      {'p', b_pawn},   {'n', b_night}, {'b', b_bishop}, {'r', b_rook},
      {'q', b_queen},  {'k', b_king},  {'P', w_Pawn},   {'N', w_Night},
      {'B', w_Bishop}, {'R', w_Rook},  {'Q', w_Queen},  {'K', w_King}};

  /**
   * @brief Remove all pieces and clear the game state.
   */
  void clear();

  /**
   * @brief Place a piece on the chessboard
   * @details This function places the specified piece on the given square of
   * the chessboard. The piece is identified by a character, and the square is
   * specified by a value from the Square enum.
   * @param square The square on the chessboard where the piece should be placed
   * @param ch The character representing the piece to be placed
   * @warning assumes the given square is empty
   */
  void place_piece(Square square, const char &ch) const;

  // piece detection
  /**
   * @brief Retrieves the piece on the specified square
   * @param sq The square on the chessboard
   * @return The character representing the piece on the square, or ' ' if the
   * square is empty
   */
  char what_piece(Square sq) const;

  /**
   * @brief Check if a square contains a white rook
   * @param sq The square to check
   * @return True if the square contains a white rook, false otherwise
   */
  bool is_white_rook(Square sq) const;

  /**
   * @brief Checks if a square contains a black rook.
   * @param sq The square to check.
   * @return True if the square contains a black rook, false otherwise.
   */
  bool is_black_rook(Square sq) const;

  /**
   * @brief Check if the given square is a rook
   * @param sq The square to check
   * @return True if the square contains a rook, False otherwise
   */
  bool is_rook(Square sq) const;

  /**
   * @brief Check if the given square is a white bishop
   * @param sq The square to check
   * @return True if the square contains a white bishop, false otherwise
   */
  bool is_white_bishop(Square sq) const;

  /**
   * @brief Check if a given square contains a black bishop
   * @param sq The square to check
   * @return True if the square contains a black bishop, false otherwise
   */
  bool is_black_bishop(Square sq) const;

  /**
   * @brief Check if the given square is a bishop
   * @param sq The square to check
   * @return True if the square contains a bishop, false otherwise
   */
  bool is_bishop(Square sq) const;

  /**
   * @brief Check if the given square is a white queen
   * @param sq The square to check
   * @return True if the square contains a white queen, false otherwise
   */
  bool is_white_queen(Square sq) const;

  /**
   * @brief Check if the given square is a black queen
   * @param sq The square to check
   * @return True if the square contains a black queen, false otherwise
   */
  bool is_black_queen(Square sq) const;

  /**
   * @brief Check if the given square is a queen
   * @param sq The square to check
   * @return True if the square contains a queen, false otherwise
   */
  bool is_queen(Square sq) const;

  /**
   * @brief Check if the given square is a white knight
   * @param sq The square to check
   * @return True if the square contains a white knight, false otherwise
   */
  bool is_white_knight(Square sq) const;

  /**
   * @brief Check if the given square is a black knight
   * @param sq The square to check
   * @return True if the square contains a black knight, false otherwise
   */
  bool is_black_knight(Square sq) const;

  /**
   * @brief Check if the given square is a knight
   * @param sq The square to check
   * @return True if the square contains a knight, false otherwise
   */
  bool is_knight(Square sq) const;

  /**
   * @brief Check if the given square is a white king
   * @param sq The square to check
   * @return True if the square contains a white king, false otherwise
   */
  bool is_white_king(Square sq) const;

  /**
   * @brief Check if the given square is a black king
   * @param sq The square to check
   * @return True if the square contains a black king, false otherwise
   */
  bool is_black_king(Square sq) const;

  /**
   * @brief Check if the given square is a king
   * @param sq The square to check
   * @return True if the square contains a king, false otherwise
   */
  bool is_king(Square sq) const;

  /**
   * @brief Check if the given square is a white pawn
   * @param sq The square to check
   * @return True if the square contains a white pawn, false otherwise
   */
  bool is_white_pawn(Square sq) const;

  /**
   * @brief Check if the given square is a black pawn
   * @param sq The square to check
   * @return True if the square contains a black pawn, false otherwise
   */
  bool is_black_pawn(Square sq) const;

  /**
   * @brief Check if the given square is a pawn
   * @param sq The square to check
   * @return True if the square contains a pawn, false otherwise
   */
  bool is_pawn(Square sq) const;

  /**
   * @brief Check if the given square is a black piece
   * @param sq The square to check
   * @return True if the square contains a black piece, false otherwise
   */
  bool is_black(Square sq) const;

  /**
   * @brief Check if the given square is a white piece
   * @param sq The square to check
   * @return True if the square contains a white piece, false otherwise
   */
  bool is_white(Square sq) const;

  /**
   * @brief Check if the given square is empty
   * @param sq The square to check
   * @return True if the square is empty, false otherwise
   */
  bool is_empty(Square sq) const;

  /**
   * @brief Check the color of a piece on the given square
   * @param sq The square to check
   * @return The color of the piece on the square (white, black, or none)
   */
  Color what_color(Square sq) const;

  /**
   * @brief Check if a two squares have the same colored pieces
   * @param sq The square to check
   * @param color The color to compare with
   * @return True if the square has the same color as the specified color, false
   * otherwise
   */
  bool is_same_color(Square sq, Color color) const;

  /**
   * @brief Checks if the enemy king is on a given square.
   * @param sq The square to check.
   * @param c Our color
   * @return True if the enemy king is present on the square, false otherwise.
   */
  bool is_opposite_king(Square sq, Color c) const;

  /**
   * @brief Get the row of a given square
   * @param sq The square to check
   * @return The number of the row
   */
  static int get_row(Square sq);

  /**
   * @brief Get the column of a given square
   * @param sq The square to check
   * @return The column number. (a:h) = (1:8)
   */
  static int get_column(Square sq);

  /**
   * @brief Check if two squares are in the same row.
   * @param sq1 The first square.
   * @param sq2 The second square.
   * @return True if the squares are in the same row, false otherwise.
   */
  static bool is_inSameRow(Square sq1, Square sq2);

  /**
   * @brief Check if three squares are in the same row
   * @param sq1 First square
   * @param sq2 Second square
   * @param sq3 Third square
   * @return True if all three squares are in the same row, false otherwise
   */
  static bool is_inSameRow(Square sq1, Square sq2, Square sq3);

  /**
   * @brief Check if two squares are in the same column
   * @param sq1 The first square
   * @param sq2 The second square
   * @return True if the squares are in the same column, false otherwise
   */
  static bool is_inSameColumn(Square sq1, Square sq2);

  /**
   * Check if three squares are in the same column.
   * @param sq1 First square.
   * @param sq2 Second square.
   * @param sq3 Third square.
   * @return True if all three squares are in the same column, false otherwise.
   */
  static bool is_inSameColumn(Square sq1, Square sq2, Square sq3);

  /**
   * @brief Check if two squares are on the same diagonal (left to right)
   * @param sq1 The first square
   * @param sq2 The second square
   * @return True if the squares are on the same diagonal false otherwise
   */
  static bool is_inSameDiagonal_leftRight(Square sq1, Square sq2);

  /**
   * @brief Check if three squares are in the same diagonal in
   * Diagonals::leftRight
   * @param sq1 The first square.
   * @param sq2 The second square.
   * @param sq3 The third square.
   * @return True if the three squares are in the same diagonal, false
   * otherwise.
   */
  static bool is_inSameDiagonal_leftRight(Square sq1, Square sq2, Square sq3);

  /**
   * @brief Check if two squares are in the same diagonal (right to left)
   * @param sq1 The first square
   * @param sq2 The second square
   * @return True if the squares are in the same diagonal, false otherwise
   */
  static bool is_inSameDiagonal_rightLeft(Square sq1, Square sq2);

  /**
   * @brief Check if three squares are in the same diagonal (right to left)
   * @param sq1 The first square
   * @param sq2 The second square
   * @param sq3 The third square
   * @return true if the three squares are in the same diagonal, false otherwise
   */
  static bool is_inSameDiagonal_rightLeft(Square sq1, Square sq2, Square sq3);

  // fen
  // fen out
  /**
   * @brief Returns the FEN representation of the piece placement on the board.
   * @return The FEN representation of the piece placement on the board.
   */
  std::string fen_piece_placement() const;

  /**
   * @brief Export the current state of the board as FEN (Forsyth–Edwards
   * Notation) string.
   * @details
   * @return FEN string for the current board state.
   */
  std::string export_fen() const;

  // fen in
  /**
   * Sets the pieces on the chessboard based on the given FEN string
   * @param fen The FEN string representing the piece placement
   * @return The number of characters processed in the FEN string
   */
  uint set_pieces(const std::string &fen) const;

  /**
   * @brief Sets the board from a given FEN string.
   * @param fen    The FEN string to import.
   */
  void import_fen(const std::string &fen);

  // begin move generation     ----------------------------------------
  /**
   * @brief Calculates the legal moves for a pawn on the board
   * @param sq The square on which the pawn is located
   * @return A vector of squares representing the legal moves for the pawn
   */
  std::vector<Square> legal_moves_pawn(Square sq) const;

  /**
   * @brief Used for getting pawn moves.
   * @param sq The square for which to calculate the legal moves
   * @return A vector of Squares representing the legal moves
   */
  std::vector<Square> legal_moves(Square sq) const;

  // influence
  /**
   * @brief Computes the influence of a given rook
   * @param sq The square to compute the influence for.
   * @return std::vector<Square> A vector of squares representing the influence
   * of the rook.
   */
  std::vector<Square> influence_rook(Square sq) const;

  /**
   * @brief Computes the infulence of a given bishop
   * @param sq The square where the bishop is located.
   * @return std::vector<Square> A vector containing all the squares influenced
   * by the bishop.
   */
  std::vector<Square> influence_bishop(Square sq) const;

  /**
   * @brief Computes the influence of a given queen
   * @param sq The square where the queen is located
   * @return std::vector<Square> The squares influenced by the queen
   */
  std::vector<Square> influence_queen(Square sq) const;

  /**
   * @brief Computes the influence of a given knight
   * @param sq The square where the knight is located.
   * @return A vector of squares influenced by the knight.
   */
  static std::vector<Square> influence_knight(Square sq);

  /**
   * @brief Computes the influence of a given king.
   * @param sq The square where the king is located.
   * @return A vector of squares that are influenced by the king.
   */
  static std::vector<Square> influence_king(Square sq);

  /**
   * @brief Calculates the squares influenced by a pawn on the chessboard
   * @param sq The square where the pawn is located
   * @return A vector of squares influenced by the pawn
   */
  std::vector<Square> influence_pawn(Square sq) const;

  /**
   * @brief Computes the influence of a given piece.
   * @param sq The square where the piece is located.
   * @return A list of squares that the piece on the given square is currently
   * "influencing".
   */
  std::vector<Square> influence(Square sq) const;

  /**
   * @brief Update the influence maps for white and black pieces
   */
  void update_influence_maps() const;

  // pinned pieces
  /**
   * @brief Finds the piece pinned by a given rook
   * @details This function searches in the vertical and horizontal directions
   * from the given square to find any potentially pinned pieces
   * @param sq The square of the rook.
   * @return The square of the pinned piece, or the square of the given rook if
   * there is no pinned piece
   */
  Square pinned_piece_rook(Square sq) const;

  /**
   * @brief Finds the piece pinned by a given bishop.
   * @details This function searches the diagonal directions outward from a
   * given bishop to see if the bishop is pinning any enemy pieces.
   * @param sq The square to start the search from.
   * @return The square of the pinned piece, or the square of the bishop if no
   * piece is pinned.
   */
  Square pinned_piece_bishop(Square sq) const;

  /**
   * @brief Finds the piece pinned by a given queen.
   * @details A queen moves like a rook and bishop combined, don't it?
   * @param sq The square to start the search from.
   * @return The square of the pinned piece, or the square of the queen if no
   * piece is pinned.
   * @see Board::pinned_piece_rook and Board::pinned_piece_bishop
   */
  Square pinned_piece_queen(Square sq) const;

  /**
   * @brief Finds the piece pinned by a given piece.
   * @param sq The square of the piece to check.
   * @return The square of the pinned piece, or the square of the piece we are
   * checking if there is no pin.
   */
  Square pinned_piece(Square sq) const;

  /**
   * @brief Update the maps of pinned pieces
   * @details Checks every piece to find pins to the enemy king and records
   * pinned pieces
   * @note mutates Maps::black_pinned and Maps::white_pinned
   */
  void update_pinned_pieces() const;

  // influence reduction
  /**
   * @brief Updates white king moves
   * @param sq_w_King The current position of the white king.
   * @return Squares containing pieces that are giving check to the white king.
   * @note May mutate Game_State::white_inCheck
   * @warning For move generation as a whole to function properly, there must be
   * exactly 1 king of each color on the board
   */
  std::vector<Square> update_white_king_moves(Square sq_w_King);

  /**
   * @brief Updates black king moves
   * @param sq_b_king The current position of the black king.
   * @return Squares containing pieces that are giving check to the black king.
   * @note May mutate Game_State::black_inCheck
   * @warning For move generation as a whole to function properly, there must be
   * exactly 1 king of each color on the board
   */
  std::vector<Square> update_black_king_moves(Square sq_b_king);

  /**
   * @brief Copies influence maps to move maps, excluding kings and pawns
   * @details Move generation is done by starting with the influence maps and
   * reducing the map by applying rules of the game. This function returns the
   * square of (a) king because we will need that info later, and now is as good
   * a time as any to get it.
   * @param c Indicates which map to copy
   * @param from The unordered_map where the squares will be copied from
   * @param to The unordered_map where the squares will be assigned
   * @return The square in which the king resides
   */
  Square
  copy_influence(Color c,
                 const std::unordered_map<Square, std::vector<Square>> *from,
                 std::unordered_map<Square, std::vector<Square>> *to) const;

  /**
   * @brief Removes same-colored squares from a given map
   * @details It is not legal to capture your own pieces
   * @param map Map to remove squares from.
   * @param color The color of the squares to be removed
   */
  void remove_same_color_squares(
      Color color, std::unordered_map<Square, std::vector<Square>> *map) const;

  /**
   * @brief Move generation.
   * @details Move maps are created by starting with all possible moves for a
   * piece using update_influence_maps(), then reducing this list by applying
   * various rules of the game
   * @note Mutates `Maps`
   * @warning There must be exactly one king of each color on on the board
   */
  void update_move_maps();
  // end move generation     ----------------------------------------

  // uci

  // diagnostic
  /**
   * Calculates the number of nodes at depth 1 for a given color.
   * @param color The color of the pieces.
   * @return The number of nodes at depth 1.
   * @note This is only in use in a test file sample-game.cxx
   */
  [[maybe_unused]] uint nodes_at_depth_1(Color color);

  // mutate board
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
  void do_move(Square from, Square to, char ch);

  /**
   * @brief Move a pawn.
   * @details Check for promotion, two square move, or en passant, then move
   * @param from The square from which the pawn is moved.
   * @param to The square to which the pawn is moved.
   * @param ch If promoting, this gives the piece to promote to.
   */
  void move_pawn(Square from, Square to, char ch);

  /**
   * @brief Removes a piece from the chessboard at the given square.
   * @details This function updates the bitboards for each piece type by
   * clearing the bit corresponding to the given square. The piece_map is then
   * updated to reflect the new bitmaps.
   * @param square The square to remove the piece from
   */
  void remove_piece(Square square);

  /**
   * @brief Move a knight, bishop, or queen.
   * @details There are no special rules to consider when moving these pieces,
   * so we can simply pick them up and set them down
   * @param from The square from which the piece is moved.
   * @param to The square to which the piece is moved.
   */
  void move_piece(Square from, Square to);

  /**
   * @brief Move a king.
   * Check for castling, update castling rights, move the king.
   * @param from The square from which the king is moved.
   * @param to The square to which the king is moved.
   */
  void move_king(Square from, Square to);

  /**
   * @brief Move a rook
   * Update castling rights, then move the piece.
   * @param from The square from which the rook is moved
   * @param to The square to which tht rook is moved
   */
  void move_rook(Square from, Square to);
};

#endif // INCLUDE_BOARD_H_
