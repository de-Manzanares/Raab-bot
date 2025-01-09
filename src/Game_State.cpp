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

#include "Game_State.h"

#include <unordered_map>

std::string Game_State::fen_castling_ability() const {
  std::string s;
  if (!castle_w_K && !castle_w_Q && !castle_b_k && !castle_b_q) {
    s += '-';
  } else {
    if (castle_w_K) {
      s += 'K';
    }
    if (castle_w_Q) {
      s += 'Q';
    }
    if (castle_b_k) {
      s += 'k';
    }
    if (castle_b_q) {
      s += 'q';
    }
  }
  return s;
}

std::string Game_State::fen_en_passant_targets() const {
  return en_passant_target.empty() ? "-" : en_passant_target;
}

std::string Game_State::fen_half_move_clock() const {
  return std::to_string(half_move_clock);
}

std::string Game_State::fen_full_move_number() const {
  return std::to_string(full_move_number);
}

void Game_State::clear() {
  half_move_clock = full_move_number = 0;
  castle_w_K = castle_w_Q = castle_b_k = castle_b_q = false;
  en_passant_target.clear();
}

void Game_State::set_castling_ability(const std::string &s) {
  if (s == "-") {
    castle_w_K = castle_w_Q = false;
    castle_b_k = castle_b_q = false;
  } else {
    std::unordered_map<char, bool &> castling_map = {{'K', castle_w_K},
                                                     {'Q', castle_w_Q},
                                                     {'k', castle_b_k},
                                                     {'q', castle_b_q}};
    for (const auto &ch : s) {
      for (auto &[key, value] : castling_map) {
        if (ch == key) {
          value = true;
        }
      }
    }
  }
}

Game_State &Game_State::operator=(const Game_State &rhs) {
  if (this != &rhs) {
    active_color = rhs.active_color;
    castle_w_K = rhs.castle_w_K;
    castle_w_Q = rhs.castle_w_Q;
    castle_b_k = rhs.castle_b_k;
    castle_b_q = rhs.castle_b_q;
    en_passant_target = rhs.en_passant_target;
    en_passant_set = rhs.en_passant_set;
    half_move_clock = rhs.half_move_clock;
    full_move_number = rhs.full_move_number;
    black_inCheck = rhs.black_inCheck;
    white_inCheck = rhs.white_inCheck;
    recent_promotion = rhs.recent_promotion;
  }
  return *this;
}
