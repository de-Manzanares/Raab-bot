/**
 * @file attack.ixx
 * is a square attacked?
 */

module;

import board;
import defs;

export module attack;

//------------------------------------------------------------------------------

/**
 * @return  true - sq is attacked \n
 *          false - sq is not attacked
 */
export bool is_attacked(const Board &b, Square sq, Color by_color);

/**
 * @return  true - stm is in check \n
 *          false - stm is not in check
 */
export bool in_check(const Board &b);

//------------------------------------------------------------------------------
