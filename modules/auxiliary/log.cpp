module;

#include "../../include/id.hpp"

#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <string>

import board;
import search;
import move;

module log;

template <class... Ts> void log(const Ts &...xs);

void log_flush();

//------------------------------------------------------------------------------

void log_search()
{
  log("info", "depth", static_cast<int>(depth), "score cp", g_eval, "nodes",
      node_count);
  if (time_elapsed != 0) {
    log("nps", node_count / time_elapsed * 1000);
  }
  log("time", time_elapsed, "pv");
  const auto last = std::ranges::find(g_pv, Move{});
  for (auto move = g_pv.begin(); move != last; ++move) {
    log(*move);
  }
  log_flush();
}

void record(const std::string_view from_gui)
{
  ofile << ">> " << from_gui << std::endl;
}

//------------------------------------------------------------------------------

template <class... Ts> void log(const Ts &...xs)
{
  ((std::cout << xs << ' '), ...);
  ((ofile << xs << ' '), ...);
}

void log_flush()
{
  std::cout << std::endl;
  ofile << std::endl;
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
  return std::string{id::name} + '-' + std::string{id::version} + time_stamp() +
         ".txt";
}
