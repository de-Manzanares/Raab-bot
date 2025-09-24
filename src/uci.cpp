module;

#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>

import Board0x88;
export module uci;

constexpr std::uint8_t max_depth = 32;

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
  while (s != "moves") {
    iss >> s;
  }
  // the following "words" in the string will be moves
  while (iss >> s) {
    move(b, to_move(b, s, ml.begin()));
    std::ranges::fill(ml, Move{});
  }
}
double time_elapsed{};

bool time_up() {}

export void uci_loop() {

  std::ofstream ofile("Raab-bot-v2-dev3-log.txt");
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
      }
      if (simon_says(&in, "moves")) {
        startpos_moves(b, &in);
      }
    }
    if (simon_says(&in, "go")) {
      int alpha = std::numeric_limits<int>::min() / 2;
      int beta = std::numeric_limits<int>::max() / 2;
      long wtime{};
      long btime{};
      long winc{};
      long binc{};
      long time{};
      long time_elapsed{};

      std::string s;
      s = "wtime";
      if (simon_says(&in, s)) {
        auto it = in.find(s);
        wtime = std::stoi(in.substr(it + 6));
      }
      s = "btime";
      if (simon_says(&in, s)) {
        auto it = in.find(s);
        btime = std::stoi(in.substr(it + 6));
      }
      s = "winc";
      if (simon_says(&in, s)) {
        auto it = in.find(s);
        winc = std::stoi(in.substr(it + 5));
      }
      s = "binc";
      if (simon_says(&in, s)) {
        auto it = in.find(s);
        binc = std::stoi(in.substr(it + 5));
      }

      if (b.stm == white && wtime != 0) {
        time = double(wtime) / 60.0 + winc / 2.0;
      } else if (b.stm == black && btime != 0) {
        time = double(btime) / 60.0 + binc / 2.0;
      }

      Move m;
      std::uint8_t depth;
      auto start = std::chrono::steady_clock::now();

      if (b.fmc < 30) {
        depth = 4U;
      } else if (b.fmc < 50) {
        depth = 5U;
      } else {
        depth = 6U;
      }

      m = alpha_beta_root(b, alpha, beta, depth, start, time, time_elapsed);
      time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                         std::chrono::steady_clock::now() - start)
                         .count();

      std::cout << "bestmove " << m << std::endl;
      ofile << "bestmove " << m << std::endl;
    } else if (in.find("stop") != std::string::npos) {
    } else if (in == "quit") {
      break;
    } // quit the loop, ends the program
  }
}
