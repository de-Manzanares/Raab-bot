#ifndef CHESSENGINE_FEN_HPP
#define CHESSENGINE_FEN_HPP

#include <string>
#include "Board.hpp"

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
std::string export_fen(const Board *board)
{
    std::string fen;    // string to be exported
    fen += board->fen_piece_placement();
    fen += ' ';
    fen += board->game_state.fen_active_color();
    fen += ' ';
    fen += board->game_state.fen_castling_ability();
    fen += ' ';
    fen += board->game_state.fen_en_passant_targets();
    fen += ' ';
    fen += board->game_state.fen_half_move_clock();
    fen += ' ';
    fen += board->game_state.fen_full_move_number();
    return fen;
}

/**
 * Sets the board from a given FEN string.
 * @param board  A pointer to the Board object.
 * @param fen    The FEN string to import.
 */
// TODO check if fen is valid before/during import
void import_fen(Board *board, const std::string& fen)
{
    std::string game_state_string;
    std::string temp;

    // clear the board
    board->clear();

    // set pieces and create game state string
    game_state_string = fen.substr(board->set_pieces(fen));

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
    active_color == 'w' ?
            board->game_state.active_color_clock = 0 : board->game_state.active_color_clock = 1;

    // castling ability
    board->game_state.set_castling_ability(castling_ability);
    board->game_state.en_passant_target = en_passant_target;
    board->game_state.half_move_clock = half_move_clock;
    board->game_state.full_move_number = full_move_number;
}

#endif //CHESSENGINE_FEN_HPP
