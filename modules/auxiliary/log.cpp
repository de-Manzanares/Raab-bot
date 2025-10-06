module;

#include "id.hpp"

#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <string>

import board;
import search;
import move;
import search;

module log;

namespace raab_bot {

template <class... Ts> void log(const Ts &...xs);

void log_flush();

//------------------------------------------------------------------------------

void log_search(SearchDriver &sd)
{
  log("info", "depth", static_cast<int>(sd.depth), "score cp", sd.eval, "nodes", sd.node_count);
  if (sd.time_elapsed != 0) {
    log("nps", sd.node_count / sd.time_elapsed * 1000);
  }
  log("time", sd.time_elapsed, "pv");
  const auto last = std::ranges::find(sd.pv, Move{});
  for (auto move = sd.pv.begin(); move != last; ++move) {
    log(*move);
  }
  std::cout << std::endl;
}

/// void record(const std::string_view from_gui)
/// {
///   ofile << ">> " << from_gui << std::endl;
/// }

//------------------------------------------------------------------------------

template <class... Ts> void log(const Ts &...xs)
{
  ((std::cout << xs << ' '), ...);
  // ((ofile << xs << ' '), ...);
}

void log_flush()
{
  std::cout << std::endl;
  // ofile << std::endl;
}

//------------------------------------------------------------------------------

std::string time_stamp()
{
  std::stringstream ss;
  const auto        now = std::chrono::system_clock::now();
  ss << std::format("_{:%Y_%m_%d_%H_%M_%S}_", now);
  return ss.str();
}

std::string log_name()
{
  return std::string{id::name} + '-' + std::string{id::version} + time_stamp() + ".txt";
}

} // namespace raab_bot
