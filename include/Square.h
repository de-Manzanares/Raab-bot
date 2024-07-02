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

#ifndef INCLUDE_SQUARE_H_
#define INCLUDE_SQUARE_H_

#include <string>
#include <type_traits>

/**
 * @enum Square
 * @brief Represents a square on a chessboard.
 *
 * The Square enumeration represents the 64 squares on a chessboard. Each square
 * is identified by its file (a-h) and rank (1-8). The underlying type of Square
 * is int.
 *
 * The squares are defined in the following order:
 *   - From h1 to a1
 *   - From h2 to a2
 *   - From h3 to a3
 *   - ...
 *   - From h8 to a8
 *
 * The values of the squares are consecutive integers from 0 to 63.
 */
// clang-format off
enum class Square : int {
    h1, g1, f1, e1, d1, c1, b1, a1,
    h2, g2, f2, e2, d2, c2, b2, a2,
    h3, g3, f3, e3, d3, c3, b3, a3,
    h4, g4, f4, e4, d4, c4, b4, a4,
    h5, g5, f5, e5, d5, c5, b5, a5,
    h6, g6, f6, e6, d6, c6, b6, a6,
    h7, g7, f7, e7, d7, c7, b7, a7,
    h8, g8, f8, e8, d8, c8, b8, a8,
};
// clang-format on

/**
 * @brief Prefix Square decrement
 * @return The decremented square
 */
Square &operator--(Square &square); // NOLINT

/**
 * @brief Prefix Square increment
 * @return The incremented square
 */
Square &operator++(Square &square); // NOLINT

/**
 * @brief Subtraction operator
 * @tparam T Must be integral type
 * @param lhs minuend
 * @param rhs subtrahend
 * @return difference
 */
template <class T> Square operator-(const Square lhs, T rhs) {
  static_assert(std::is_integral_v<T>);
  Square copy = lhs;
  for (auto i = 0; i < rhs; i++) {
    rhs > 0 ? --copy : ++copy;
  }
  return copy;
}

/**
 * @brief Addition operator
 * @tparam T Must be integral type
 * @param lhs addend
 * @param rhs addend
 * @return sum
 */
template <class T> Square operator+(Square lhs, T rhs) {
  static_assert(std::is_integral_v<T>);
  Square copy = lhs;
  for (auto i = 0; i < rhs; i++) {
    rhs > 0 ? ++copy : --copy;
  }
  return copy;
}

/**
 * @enum Color
 * @brief Color of a piece : white, black, or none (absence of a piece)
 */
enum class Color { white, black, none };

/**
 * @brief Color negation operator
 * @return The negated color
 * @note There is no negation for Color::none
 */
Color &operator!(Color &color); // NOLINT

/**
 * @struct Sq
 * @brief For converting between string representations and enum values of
 * Square.
 */
struct Sq {
  /**
   * @brief std::string "xy" to Square xy
   * @param string The string representation of the square.
   * @return The Square representation of the square.
   */
  static Square string_to_square(const std::string &string);

  /**
   * @brief Square xy to std::string "xy"
   * @param square The Square representation of the square
   * @return The std::string representation of the square.
   */
  static std::string square_to_string(const Square &square);
};

#endif // INCLUDE_SQUARE_H_
