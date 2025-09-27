module;

#include "../../include/id.hpp"

#include <chrono>
#include <iostream>
#include <string>

import board;
import move;
import types;
import fen;
import search;
import movegen;

import log;

module uci;

constexpr score_t alpha = std::numeric_limits<score_t>::min() / 2; ///< lb
constexpr score_t beta  = std::numeric_limits<score_t>::max() / 2; ///< ub

void preamble(const std::string *in);
bool simon_says(const std::string *s, const std::string &has);
void startpos_moves(Board &b, const std::string *in);
Move to_move(const Board &b, std::string_view s, MlIt out);

//------------------------------------------------------------------------------

void uci_loop() {
  std::string in; // the command from the GUI
  Board       b;
  while (std::getline(std::cin, in)) {
    record(in);
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
        wtime   = std::stoi(in.substr(it + 6));
      }
      s = "btime";
      if (simon_says(&in, s)) {
        auto it = in.find(s);
        btime   = std::stoi(in.substr(it + 6));
      }
      s = "winc";
      if (simon_says(&in, s)) {
        auto it = in.find(s);
        winc    = std::stoi(in.substr(it + 5));
      }
      s = "binc";
      if (simon_says(&in, s)) {
        auto it = in.find(s);
        binc    = std::stoi(in.substr(it + 5));
      }
      if (b.stm == white && wtime != 0) {
        allowed_time = double(wtime) / 60.0 + winc;
      } else if (b.stm == black && btime != 0) {
        allowed_time = double(btime) / 60.0 + binc;
      } else {
        allowed_time = 1000;
      }
      s = "movetime";
      if (simon_says(&in, s)) {
        auto it      = in.find(s);
        allowed_time = std::stoi(in.substr(it + 9));
      }

      start = std::chrono::steady_clock::now();
      for (depth = 1; depth <= max_depth; ++depth) {
        alpha_beta_root(b, alpha, beta, depth);
        if (root_trees <= rte || root_beta_cutoff) {
          log_search();
        } else {
          break;
        }
        if (g_pv[0] != Move{}) {
          prev_g_pv = g_pv;
        }
      }

      logln("bestmove", prev_g_pv[0]);

    } else if (in.find("stop") != std::string::npos) {
    } else if (in == "d") {
      b.display();
    } else if (in == "quit") {
      break;
    } // quit the loop, ends the program
  }
}

//------------------------------------------------------------------------------

// for some reason, Scid vs PC is very sensitive to the format of the preamble
void preamble(const std::string *in) {
  if (*in == "uci") {
    std::cout << "id name Raab-bot\nid author Schauss\nuciok\n";
  } else if (*in == "isready") {
    std::cout << "readyok\n";
  }
}

// void preamble(const std::string *in) {
//   if (*in == "uci") {
//     logln("id name", id::name, id::version);
//     logln("id author", id::author);
//     logln("uciok");
//   } else if (*in == "isready") {
//     logln("readyok");
//   }
// }
//

bool simon_says(const std::string *s, const std::string &has) {
  return s->find(has) != std::string::npos;
}

void startpos_moves(Board &b, const std::string *in) {
  MoveList           ml;
  std::istringstream iss(*in);
  std::string        s;
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

Move to_move(const Board &b, const std::string_view s, const MlIt out) {
  auto       ch = s.begin();
  const auto from_sq =
      static_cast<Square>((16 * (*std::next(ch) - '0' - 1)) + *ch - 'a');
  std::advance(ch, 2);
  if (*ch == 'x') {
    std::advance(ch, 1);
  }
  const auto to_sq =
      static_cast<Square>((16 * (*std::next(ch) - '0' - 1)) + *ch - 'a');
  std::advance(ch, 2);
  auto prom_p = null_piece;
  if (ch != s.end()) {
    prom_p = fen::get_piece_info(*ch).piece_t;
  }
  movegen(b, out);
  for (auto m = out; m->from_sq != null_square; ++m) {
    if (m->from_sq == from_sq && m->to_sq == to_sq && m->prom_p == prom_p) {
      return *m;
    }
  }
  throw(std::invalid_argument("move not found in move list")); //< todo catch
}
