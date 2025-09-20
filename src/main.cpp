#include <iostream>

import Board0x88;

int main() {
  Board b("5K2/8/5kp1/7r/8/8/3b4/8 b - - 5 66");
  std::array<Move, 1024> ml{};
  auto mv = negamax_root(b, 4);
}