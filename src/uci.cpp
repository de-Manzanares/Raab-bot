module;

#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>

import Board0x88;
export module uci;

bool simon_says(const std::string *s, const std::string &has) {
  return s->find(has) != std::string::npos;
}

template <class OutputIt>
  requires std::output_iterator<OutputIt, Move>
Move to_move(Board &b, std::string_view s, OutputIt out) {
  auto ch = s.begin();
  auto from_sq =
      static_cast<Square>((16 * (*std::next(ch) - '0' - 1)) + *ch - 'a');
  std::advance(ch, 2);
  if (*ch == 'x') {
    std::advance(ch, 1);
  }
  auto to_sq =
      static_cast<Square>((16 * (*std::next(ch) - '0' - 1)) + *ch - 'a');
  auto count = movegen(b, out);
  for (auto m = out; m->from_sq != null_square; ++m) {
    if (m->from_sq == from_sq && m->to_sq == to_sq) {
      return *m;
    }
  }
}

void preamble(const std::string *in) {
  if (*in == "uci") {
    std::cout << "id name Raab-bot\nid author Schauss\nuciok\n";
  } else if (*in == "isready") {
    std::cout << "readyok\n";
  }
}

void startpos_moves(Board &b, const std::string *in) {
  std::array<Move, 256> ml;
  std::istringstream iss(*in);
  std::string s;
  // get rid of "position startpos moves" so we can process the moves
  for (auto i = 0; i < 3; i++) {
    iss >> s;
  }
  // the following "words" in the string will be moves
  while (iss >> s) {
    move(b, to_move(b, s, ml.begin()));
    std::ranges::fill(ml, Move{});
  }
}

export void uci_loop() {
  std::ofstream ofile("Raab-bot-v2-dev1-log.txt");
  std::string in; // the command from the GUI
  Board b;
  while (std::getline(std::cin, in)) {
    ofile << in << std::endl;
    preamble(&in);
    if (simon_says(&in, "position")) {
      if (simon_says(&in, "fen")) {
        b = Board{in.substr(13)};
      } else if (simon_says(&in, "startpos")) {
        b.reset();
        if (simon_says(&in, "moves")) {
          startpos_moves(b, &in);
        }
      }
    }
    if (simon_says(&in, "go")) {
      constexpr int alpha = std::numeric_limits<int>::min() / 2;
      constexpr int beta = std::numeric_limits<int>::max() / 2;
      auto m = alpha_beta_root(b, alpha, beta, 4);
      std::cout << "bestmove " << m << std::endl;
      ofile << "bestmove " << m << std::endl;
    } else if (in.find("stop") != std::string::npos) {
    } else if (in == "quit") {
      break;
    } // quit the loop, ends the program
  }
}
