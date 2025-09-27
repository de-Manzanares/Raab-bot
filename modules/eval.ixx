module;

import board;
import types;
import transposition;

export module eval;

//------------------------------------------------------------------------------

// arbitrary values should be powers of 2 from now on LOL

export constexpr score_t CHECKMATE = 1ULL << 30; ///< checkmate score

/**
 * evaluate a static position
 * @return  score: \n
 *          score < 0 opponent advantage (~stm)
 *          score = 0 draw
 *          score > 0 stm advantage
 */
export score_t static_eval(Board &b);

/**
 * should we draw or push for the win?
 */
export score_t contempt(const Board &b);

//------------------------------------------------------------------------------
