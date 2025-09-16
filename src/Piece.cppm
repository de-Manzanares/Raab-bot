/**
 * @file Piece.cppm
 */

module;

export module Piece;

// todo possibly modernize this interface
// it is a nice way to loop through everthing though ... I guess ...
// quick easy access

export enum Direction : int { NW = 15, N, NE, E = -1, W = 1, SW = -17, S, SE };

export enum Piece : int { king, queen, rook, bishop, knight, pawn, empty };

bool is_sliding[5] = {false, true, true, true, false};

export constexpr int n_vectors[5] = {8, 8, 4, 4, 8};
export constexpr int vectors[5][8] = {{SW, S, SE, E, W, NW, N, NE},
                                      {SW, S, SE, E, W, NW, N, NE},
                                      {S, E, W, N},
                                      {SW, SE, NW, NE},
                                      {-33, -31, -18, -14, 14, 18, 31, 33}};
