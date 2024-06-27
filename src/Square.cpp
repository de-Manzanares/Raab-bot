#include "../include/Square.h"

Square &operator--(Square &square) { // NOLINT
  square = static_cast<Square>(static_cast<int>(square) - 1);
  return square;
}

Square &operator++(Square &square) // NOLINT
{
  square = static_cast<Square>(static_cast<int>(square) + 1);
  return square;
}

Color &operator!(Color &color) // NOLINT
{
  color == Color::white ? color = Color::black : color = Color::white;
  return color;
}

std::unordered_map<std::string, Square> string_square = {
    {"h1", Square::h1}, {"g1", Square::g1}, {"f1", Square::f1},
    {"e1", Square::e1}, {"d1", Square::d1}, {"c1", Square::c1},
    {"b1", Square::b1}, {"a1", Square::a1}, {"h2", Square::h2},
    {"g2", Square::g2}, {"f2", Square::f2}, {"e2", Square::e2},
    {"d2", Square::d2}, {"c2", Square::c2}, {"b2", Square::b2},
    {"a2", Square::a2}, {"h3", Square::h3}, {"g3", Square::g3},
    {"f3", Square::f3}, {"e3", Square::e3}, {"d3", Square::d3},
    {"c3", Square::c3}, {"b3", Square::b3}, {"a3", Square::a3},
    {"h4", Square::h4}, {"g4", Square::g4}, {"f4", Square::f4},
    {"e4", Square::e4}, {"d4", Square::d4}, {"c4", Square::c4},
    {"b4", Square::b4}, {"a4", Square::a4}, {"h5", Square::h5},
    {"g5", Square::g5}, {"f5", Square::f5}, {"e5", Square::e5},
    {"d5", Square::d5}, {"c5", Square::c5}, {"b5", Square::b5},
    {"a5", Square::a5}, {"h6", Square::h6}, {"g6", Square::g6},
    {"f6", Square::f6}, {"e6", Square::e6}, {"d6", Square::d6},
    {"c6", Square::c6}, {"b6", Square::b6}, {"a6", Square::a6},
    {"h7", Square::h7}, {"g7", Square::g7}, {"f7", Square::f7},
    {"e7", Square::e7}, {"d7", Square::d7}, {"c7", Square::c7},
    {"b7", Square::b7}, {"a7", Square::a7}, {"h8", Square::h8},
    {"g8", Square::g8}, {"f8", Square::f8}, {"e8", Square::e8},
    {"d8", Square::d8}, {"c8", Square::c8}, {"b8", Square::b8},
    {"a8", Square::a8}};

/**
 * @brief Convert a string representation of a square to a Square enum value.
 * @param string The string representation of the square.
 * @return The corresponding Square enum value.
 */
Square Sq::string_to_square(const std::string &string) {
  Square square{};

  for (const auto &[s, sq] : string_square) {
    if (string == s) {
      square = sq;
      break;
    }
  }
  return square;
}

/**
 * @brief Converts a Square enum value to its corresponding string
 * representation.
 * @param square The Square enum value to convert.
 * @return std::string The string representation of the Square enum value.
 */
std::string Sq::square_to_string(const Square &square) {
  std::string string{};

  for (const auto &[s, sq] : string_square) {
    if (square == sq) {
      string = s;
      break;
    }
  }
  return string;
}
