module;

#include <iostream>

import defs;

export module transposition;

//------------------------------------------------------------------------------

/**
 * @class Zobrist
 * all the numbers we need for Zobrist hashing
 */
export class Zobrist {
 public:
  Zobrist();
  U64 pcs[6][2][128]; ///< piece color squares
  U64 stm{};          ///< side to move
  U64 cr[16];         ///< castling rights
  U64 ep[128];        ///< ep
  U64 mat[6][2];      ///< piece color, for material hash
};

export Zobrist zobrist; ///< global Zobrist numbers

/// for categorizing ttable entries
export enum TT_flag : U8 { tt_exact, tt_alpha, tt_beta, null_tt_flag };

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
  U64     hash{};
  TT_move tt_m{};
  score_t score{};
  TT_flag flag = null_tt_flag;
  U8      depth{};
};

export constexpr U32 tt_size = (1ULL << 21U); ///< entries in the ttable
export constexpr U32 tt_mask = tt_size - 1;   ///< for quick indexing

export TT_entry tt[tt_size]; ///< global transposition table

export struct MT_entry {
  U64     m_hash;
  score_t mat; ///< material balance
};

export constexpr U32 mt_size = (1ULL << 18U); ///< entries in the table
export constexpr U32 mt_mask = mt_size - 1;   ///< for quick indexing

/**
 * @warning only valuable for caching results of values that aren't
 *          incrementally updated with move and unmove
 */
export MT_entry mt[mt_size];

export template <class Class> void print_tt_size_info()
{
  constexpr auto sz = static_cast<double>(sizeof(Class));
  constexpr auto mb = static_cast<double>(1024 * 1024);
  for (U32 i = 1; i < 33; ++i) {
    const auto n_entries = 1ULL << i;
    std::cout << 2 << " ^ " << i << " = " << n_entries << " entries "
              << static_cast<double>(n_entries) * sz / mb << " MB "
              << std::endl;
  }
}

//------------------------------------------------------------------------------
