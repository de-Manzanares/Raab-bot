module;

#include <fstream>
#include <iostream> // used by log.inl

import search;

export module log;

/// creates a time-stamped name for the log file
std::string log_name();

/// log file - records all gui input and engine output
// std::ofstream ofile("rbv2dev7_log");

//------------------------------------------------------------------------------

/**
 * print arguments to cout and the log file
 */
export template <class... Ts> void logln(const Ts &...xs);

/**
 * print search update to cout and the log file
 */
export void log_search(SearchDriver &sd);

/**
 * record gui input to the log file
 */
export void record(std::string_view from_gui);

//------------------------------------------------------------------------------

#include "log.inl"
