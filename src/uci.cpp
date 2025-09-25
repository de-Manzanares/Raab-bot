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

constexpr int alpha = std::numeric_limits<int>::min() / 2;
constexpr int beta = std::numeric_limits<int>::max() / 2;

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

  std::advance(ch, 2);
  auto prom_p = null_piece;
  if (ch != s.end()) {
    prom_p = fen::get_piece_info(*ch).piece_t;
  }
  auto count = movegen(b, out);
  for (auto m = out; m->from_sq != null_square; ++m) {
    if (m->from_sq == from_sq && m->to_sq == to_sq && m->prom_p == prom_p) {
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
      long wtime{};
      long btime{};
      long winc{};
      long binc{};

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
        allowed_time = double(wtime) / 60.0 + winc;
      } else if (b.stm == black && btime != 0) {
        allowed_time = double(btime) / 60.0 + binc;
      } else {
        allowed_time = 1000;
      }

      start = std::chrono::steady_clock::now();

      for (uint depth = 1; depth <= max_depth; ++depth) {
        try {
          alpha_beta_root(b, alpha, beta, depth);
        } catch (const std::exception &e) {
        }
        time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                           std::chrono::steady_clock::now() - start)
                           .count();

        // std::cout << "root trees searched: " << root_trees_searched
        //           << std::endl;
        // if we finished the layer or got a root beta cutoff
        if (root_trees <= root_trees_searched || root_beta_cutoff) {
          std::cout << "info ";
          std::cout << "depth " << static_cast<int>(depth) << ' ';
          std::cout << "pv ";
          ofile << "info ";
          ofile << "depth " << static_cast<int>(depth) << ' ';
          ofile << "pv ";
          for (int i = 0;; ++i) {
            if (g_pv[i].from_sq == null_square) {
              break;
            }
            std::cout << g_pv[i] << ' ';
            ofile << g_pv[i] << ' ';
          }
          std::cout << std::endl;
          ofile << std::endl;
        } else {
          break;
        }
        if (g_pv[0] != Move{}) {
          prev_g_pv = g_pv;
        }
      }
      std::cout << "bestmove " << prev_g_pv[0] << std::endl;
      ofile << "bestmove " << prev_g_pv[0] << std::endl;
      // std::cout << "elapsed time "
      //           << std::chrono::duration_cast<std::chrono::milliseconds>(
      //                  std::chrono::steady_clock::now() - start);

    } else if (in.find("stop") != std::string::npos) {
    } else if (in == "d") {
      b.display();
    } else if (in == "quit") {
      break;
    } // quit the loop, ends the program
  }
}
