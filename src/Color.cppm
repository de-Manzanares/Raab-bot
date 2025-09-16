/**
 * @file Color.cppm
 * Color type
 */

export module Color;

export enum class Color { white, black };

Color operator~(const Color color) {
  if (color == Color::white) {
    return Color::black;
  }
  return Color::white;
}
