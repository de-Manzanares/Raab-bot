module;

#include "id.hpp"

#include <cassert>
#include <chrono>
#include <iostream>
#include <string>

import board;
import config;
import defs;
import move;
import fen;
import movegen;
import search;
import transposition;

import log;

module uci;

namespace raab_bot {

constexpr score_t alpha = std::numeric_limits<score_t>::min() / 2; ///< lb
constexpr score_t beta  = std::numeric_limits<score_t>::max() / 2; ///< ub

template <typename T> bool accept_value(const std::string &sup_s, std::string_view sub_s, T &var);

void preamble(const std::string *in);
bool simon_says(const std::string *s, const std::string &has);
void startpos_moves(Board &b, const std::string *in);
Move to_move(const Board &b, std::string_view s, MlIt out);

//------------------------------------------------------------------------------

void uci_loop()
{
  std::string gui_cmd; // the command from the GUI
  Board       b;
  while (std::getline(std::cin, gui_cmd)) {
    // record(gui_cmd);
    preamble(&gui_cmd);
    if (simon_says(&gui_cmd, "position")) {
      if (simon_says(&gui_cmd, "fen")) {
        // todo probably shouldn't erase history every single time
        auto h    = std::move(b.history);
        b         = Board{gui_cmd.substr(13)};
        b.history = std::move(h);
        // eh? EH!?
      }
      else if (simon_says(&gui_cmd, "startpos")) {
        // todo probably shouldn't erase history every single time
        auto h = std::move(b.history);
        b.reset();
        b.history = std::move(h);
      }
      if (simon_says(&gui_cmd, "moves")) {
        startpos_moves(b, &gui_cmd);
      }
    }
    if (simon_says(&gui_cmd, "go")) {
      SearchDriver sd;
      b.history_decay();

      long wtime{};
      long winc{};
      long btime{};
      long binc{};

      accept_value(gui_cmd, "wtime", wtime);
      accept_value(gui_cmd, "btime", btime);
      accept_value(gui_cmd, "winc", winc);
      accept_value(gui_cmd, "binc", binc);

      // my little time per move function
      if (b.stm == white && wtime != 0) {
        sd.allowed_time = double(wtime) / 60.0 + winc;
      }
      else if (b.stm == black && btime != 0) {
        sd.allowed_time = double(btime) / 60.0 + binc;
      }
      else {
        sd.allowed_time = 1000;
      }

      U8 target_depth = config::params::max_depth;

      if (accept_value(gui_cmd, "go movetime", sd.allowed_time)) {
        ;
      }
      else if (accept_value(gui_cmd, "go depth", target_depth)) {
        sd.allowed_time = std::numeric_limits<decltype(sd.allowed_time)>::max();
      }
      else if (gui_cmd.find("go infinite") != std::string::npos) {
        target_depth    = std::numeric_limits<decltype(target_depth)>::max();
        sd.allowed_time = std::numeric_limits<decltype(sd.allowed_time)>::max();
      }

      // so that we don't time out before recreating the pv
      sd.prev_pv[0] = Move{};

      sd.start = std::chrono::steady_clock::now();

      alpha_beta_root(b, alpha, beta, sd);

      for (sd.depth = 2; sd.depth <= target_depth; ++sd.depth) {
        alpha_beta_root(b, alpha, beta, sd);
        if (sd.root_trees <= sd.root_trees_examined || sd.root_beta_cutoff) {
          log_search(sd);
        }
        else {
          break;
        }
        if (sd.pv[0] != Move{}) {
          sd.prev_pv   = sd.pv;
          sd.prev_eval = sd.eval;
        }
      }

      // logln("bestmove", sd.prev_pv[0]);
      std::cout << "bestmove " << sd.prev_pv[0] << std::endl;
    }
    else if (gui_cmd.find("stop") != std::string::npos) {
    }
    else if (gui_cmd == "d") {
      b.display();
    }
    else if (simon_says(&gui_cmd, "ucinewgame")) {
      for (U32 i = 0; i < tt_size; ++i) {
        tt[i] = TT_entry{};
      }
      b.reset();
    }
    else if (gui_cmd == "quit") {
      break;
    } // quit the loop, ends the program
  }
}

//------------------------------------------------------------------------------

template <typename T>
bool accept_value(const std::string &sup_s, const std::string_view sub_s, T &var)
{
  if constexpr (std::is_integral_v<T>) {
    if (const auto it = sup_s.find(sub_s); it != std::string::npos) {
      var = std::stoi(sup_s.substr(it + sub_s.size() + 1));
      return true;
    }
  }
  return false;
}

// for some reason, Scid vs. PC is very sensitive to the format of the preamble
void preamble(const std::string *in)
{
  if (*in == "uci") {
    std::cout << "id name Raab-bot\nid author Schauss\nuciok" << std::endl;
  }
  else if (*in == "isready") {
    std::cout << "readyok" << std::endl;
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

bool simon_says(const std::string *s, const std::string &has)
{
  return s->find(has) != std::string::npos;
}

void startpos_moves(Board &b, const std::string *in)
{
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

Move to_move(const Board &b, const std::string_view s, const MlIt out)
{
  auto       ch      = s.begin();
  const auto from_sq = static_cast<Square>((16 * (*std::next(ch) - '0' - 1)) + *ch - 'a');
  std::advance(ch, 2);
  if (*ch == 'x') {
    std::advance(ch, 1);
  }
  const auto to_sq = static_cast<Square>((16 * (*std::next(ch) - '0' - 1)) + *ch - 'a');
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

} // namespace raab_bot
