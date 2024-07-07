/*
 *     ____              __          __          __
 *    / __ \____ _____ _/ /_        / /_  ____  / /_
 *   / /_/ / __ `/ __ `/ __ \______/ __ \/ __ \/ __/
 *  / _, _/ /_/ / /_/ / /_/ /_____/ /_/ / /_/ / /_
 * /_/ |_|\__,_/\__,_/_.___/     /_.___/\____/\__/
 *
 * Copyright (c) 2024 de-Manzanares
 * This work is released under the MIT license.
 *
 */

#include "UCI.h"
#include "Eval.h"
#include "Search.h"

#include <cmath>
#include <iostream>
#include <ranges>

namespace uciloop {

enum class Time_Control { bullet, blitz, rapid };

bool is_maxing(const std::shared_ptr<Node> &n) {
  return n->active_color() == Color::white;
}

bool simon_says(const std::string *s, const std::string &has) {
  return s->find(has) != std::string::npos;
}

// parameters
double neg_inf = -100'000;
double pos_inf = 100'000;

bool continue_status_updates;

void status_update_thread(const uint update_interval_ms) {
  uint previous = 0;
  Counter::start = std::chrono::high_resolution_clock::now();
  uint counter = 0;

  while (continue_status_updates) { // NOLINT
    counter++;
    if (counter == 100) {
      counter = 0;
    }
    if (counter == 0) {
      const uint current = Counter::node;
      auto now = std::chrono::high_resolution_clock::now();

      std::cout << "info"
                // << " depth "
                << " time "
                << std::chrono::duration_cast<std::chrono::milliseconds>(
                       now - Counter::start)
                       .count()
                << " nodes " << Counter::node << " nps " << current - previous
                << std::endl; // GOTTA FLUSH THE BUFFER!!!
      previous = current;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(update_interval_ms));
  }
}

void string_to_move(const std::string *string, Square *from, Square *to,
                    char *ch) {
  std::string temp;
  auto count = 0;
  for (const auto c : *string) {
    if (c != 'x' && c != '#') {
      temp.push_back(c);
      count++;
    }
    if (count == 2) {
      *from = Sq::string_to_square(temp);
      temp.clear();
    } else if (count == 4) {
      *to = Sq::string_to_square(temp);
      temp.clear();
    } else if (count == 5) {
      *ch = c;
    }
  }
}

std::string long_algebraic_notation(const std::shared_ptr<Node> &n) {
  std::string s;
  s += Sq::square_to_string(n->from()) += Sq::square_to_string(n->to());
  if (n->promotion() != 0) {
    s += n->promotion();
  }
  return s;
}

void preamble(const std::string *in) {
  // should give engine options to be configured ... we don't have any right
  // now, so ... uciok!
  if (*in == "uci") {
    std::cout << "id name Raab-bot\nid author Schauss\nuciok\n";
  }

  // to give the engine time to set up stuff ... but we don't have any stuff!!!
  else if (*in == "isready") {
    std::cout << "readyok\n";
  }
}

void startpos_moves(const std::shared_ptr<Node> &n, const std::string *in) {
  std::istringstream iss(*in);
  std::string s;

  // get rid of "position startpos moves" so we can process the moves
  for (auto i = 0; i < 3; i++) {
    iss >> s;
  }

  // the following "words" in the string will be moves
  while (iss >> s) {
    Square from{}, to{};
    char ch{};
    string_to_move(&s, &from, &to, &ch);
    n->board()->do_move(from, to, ch);
  }
}

} // namespace uciloop

uint uci::DEPTH = 3;
uciloop::Time_Control time_control;

void uci::loop() {
  namespace ulp = uciloop;
  std::string in;                    // the command from the GUI
  std::shared_ptr<Node> n = nullptr; // root node is a pointer for easy deletion
  // and rebuilding of the decision tree

  while (std::getline(std::cin, in)) {
    ulp::preamble(&in);

    // set up a board in the given position, wait for "go" to search, a fen or a
    // list of moves may follow
    if (ulp::simon_says(&in, "position")) {
      if (ulp::simon_says(&in, "fen")) {
      } else if (ulp::simon_says(&in, "startpos")) {
        n = std::make_shared<Node>();
        if (ulp::simon_says(&in, "moves")) {
          ulp::startpos_moves(n, &in);
        }
      }
    } else if (ulp::simon_says(&in, "go") && n != nullptr) {
      constexpr uint SECONDS = 1'000;
      constexpr uint MINUTES = 60 * SECONDS;
      uint wtime{};
      uint btime{};
      Counter::node = 0;                   // reset counter
      ulp::continue_status_updates = true; // reset flag
      const bool maxing = ulp::is_maxing(n);
      n->board()->update_move_maps();

      if (uciloop::simon_says(&in, "wtime")) { // get times
        std::istringstream iss(in);
        std::string s;
        iss >> s >> s >> wtime >> s >> btime;
      }

      // get time control
      // set once at beginning of game
      if (n->board()->game_state.full_move_number < 2) {
        const auto our_time = maxing ? wtime : btime;
        if (our_time >= 1 * MINUTES) {
          time_control = ulp::Time_Control::bullet;
        }
        if (our_time >= 3 * MINUTES) {
          time_control = ulp::Time_Control::blitz;
        }
        if (our_time >= 10 * MINUTES) {
          time_control = ulp::Time_Control::rapid;
        }
      }

      double movecount = 0;
      for (const auto &moves :
           n->board()->maps->white_moves | std::views::values) {
        movecount += static_cast<double>(moves.size());
      }
      for (const auto &moves :
           n->board()->maps->black_moves | std::views::values) {
        movecount += static_cast<double>(moves.size());
      }
      // complexity switch
      double NODE_LIMIT;
      // set time limit based on time control
      switch (time_control) {
        using tc = uciloop::Time_Control;
      case tc::bullet:
      case tc::blitz:
        NODE_LIMIT = 2'000'000;
        break;
      case tc::rapid:
        NODE_LIMIT = 4'000'000;
        break;
      default:
        NODE_LIMIT = 2'000'000;
      }

      DEPTH = 3;

      if (constexpr double NPS = 50'000;
          n->board()->game_state.full_move_number >= 12 &&
          (maxing && wtime >= 2 * (NODE_LIMIT / NPS) * SECONDS ||
           !maxing && btime >= 2 * (NODE_LIMIT / NPS) * SECONDS)) {
        for (uint i = 4; i < 31; i++) {
          if (std::pow(movecount / 2, i) <= NODE_LIMIT) {
            DEPTH = i;
          } else {
            break;
          }
        }
      }

      std::thread status_thread(ulp::status_update_thread, 10);
      n->spawn_depth_first(DEPTH);

      ulp::continue_status_updates = false;
      status_thread.join();

      const auto opt_node = std::make_shared<Node>(
          *Search::min_max(n, DEPTH, ulp::neg_inf, ulp::pos_inf, maxing));

      const auto time =
          std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::high_resolution_clock::now() - Counter::start)
              .count();

      std::cout << "info"
                << " depth " << opt_node->node_depth() << " time " << time
                << " nodes " << Counter::node << "\nbestmove "
                << ulp::long_algebraic_notation(n->next_step(opt_node))
                << std::endl;

      n.reset();

    } else if (in.find("stop") != std::string::npos) {
    } else if (in == "quit") {
      break;
    } // quit the loop, ends the program
  }
}
