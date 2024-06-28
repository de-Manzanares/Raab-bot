#include "Square.h"

#include <unordered_map>

using s = Square;

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
    {"h1", s::h1}, {"g1", s::g1}, {"f1", s::f1}, {"e1", s::e1}, {"d1", s::d1},
    {"c1", s::c1}, {"b1", s::b1}, {"a1", s::a1}, {"h2", s::h2}, {"g2", s::g2},
    {"f2", s::f2}, {"e2", s::e2}, {"d2", s::d2}, {"c2", s::c2}, {"b2", s::b2},
    {"a2", s::a2}, {"h3", s::h3}, {"g3", s::g3}, {"f3", s::f3}, {"e3", s::e3},
    {"d3", s::d3}, {"c3", s::c3}, {"b3", s::b3}, {"a3", s::a3}, {"h4", s::h4},
    {"g4", s::g4}, {"f4", s::f4}, {"e4", s::e4}, {"d4", s::d4}, {"c4", s::c4},
    {"b4", s::b4}, {"a4", s::a4}, {"h5", s::h5}, {"g5", s::g5}, {"f5", s::f5},
    {"e5", s::e5}, {"d5", s::d5}, {"c5", s::c5}, {"b5", s::b5}, {"a5", s::a5},
    {"h6", s::h6}, {"g6", s::g6}, {"f6", s::f6}, {"e6", s::e6}, {"d6", s::d6},
    {"c6", s::c6}, {"b6", s::b6}, {"a6", s::a6}, {"h7", s::h7}, {"g7", s::g7},
    {"f7", s::f7}, {"e7", s::e7}, {"d7", s::d7}, {"c7", s::c7}, {"b7", s::b7},
    {"a7", s::a7}, {"h8", s::h8}, {"g8", s::g8}, {"f8", s::f8}, {"e8", s::e8},
    {"d8", s::d8}, {"c8", s::c8}, {"b8", s::b8}, {"a8", s::a8}};

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
