module;

import types;

export module transposition;

//------------------------------------------------------------------------------

export constexpr U32 tt_size = (1ULL << 21U); ///< entries in the ttable
export constexpr U32 tt_mask = tt_size - 1;   ///< for quick indexing

/**
 * @class Zobrist
 * all the numbers we need for Zobrist hashing
 */
export class Zobrist {
 public:
  Zobrist();
  U64 piece_square[6][2][128];
  U64 stm{};
  U64 castling[16];
  U64 ep[128];
};

export Zobrist zobrist; ///< global Zobrist numbers

/// for categorizing ttable entries
export enum TT_flag : U8 { tt_exact, tt_alpha, tt_beta };

/// store moves in the ttable
export struct TT_move {
  I8   from   = -1;
  I8   to     = -1;
  U8   prom_p = '.';
  bool operator==(const TT_move &rhs) const;
  bool operator!=(const TT_move &rhs) const;
};

/// ttable entries //< todo reduce to 16 bytes
export struct TT_entry {
  U64     hash;
  TT_move tt_m;
  score_t score;
  TT_flag flag;
  U8      depth;
};

export TT_entry tt[tt_size]; ///< global transposition table

export void print_tt_size_info();

//------------------------------------------------------------------------------
