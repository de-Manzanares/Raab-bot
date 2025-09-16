#include <iostream>

import Attack;
import Board;
import chess.types;

int main() {
  Board board1("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  Board board2("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
  Board board3("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b - e3 0 1");
  Board board0;
  board1.display();
  std::cout << '\n';
  board2.display();
  std::cout << '\n';
  board0.display();
  if (is_attacked(board0, a6, black)) {
    std::cout << "a6 is attacked by black \n";
  }
}