/**
 * @file Color.cppm
 * Color type
 */

export module Color;

export enum Color { white, black, empty };

Color operator~(const Color color) {
  if (color == white) {
    return black;
  }
  return white;
}
