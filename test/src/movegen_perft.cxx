#include <catch2/catch_all.hpp>

#include <chrono>
#include <iostream>

// https://www.chessprogramming.org/Perft_Results

import all_modules;

sz_t perft_results[7][7] = {
    {0,  0,    0,     0,       0,         0,          0},
    {0, 20,  400,  8902,  197281,   4865609,  119060324},
    {0, 48, 2039, 97862, 4085603, 193690690, 8031647685},
    {0, 14,  191,  2812,   43238,    674624,   11030083},
    {0,  6,  264,  9467,  422333,  15833292,  706045033},
    {0, 44, 1486, 62379, 2103487,  89941194,          0},
    {0, 46, 2079, 89890, 3894594, 164075551, 6923051137}
};

struct PerftCounts {
  sz_t captures{};
  sz_t ep{};
  sz_t castles{};
  sz_t promotions{};
  sz_t checks{};
  sz_t checkmates{};
};

void cnt_mv_t(const Move &m, PerftCounts *pc)
{
  switch (m.flag) {
  case capture:
    pc->captures++;
    break;
  case en_passant_capture:
    pc->captures++;
    pc->ep++;
    break;
  case castle:
    pc->castles++;
    break;
  case promotion:
    pc->promotions++;
    break;
  default:
  }
}

constexpr sz_t movegen_perft(Board &b, const int depth,
                             PerftCounts *pc = nullptr)
{
  MoveList ml;
  sz_t     nodes{};
  if (depth == 0) {
    return 1ULL;
  }
  movegen(b, ml.begin());
  for (const auto &m : ml) {
    if (m.from_sq == null_square) {
      break;
    }
    auto hasha     = b.t_hash;
    auto m_hash_a  = b.m_hash;
    auto mat_bal_a = b.mat_bal;
    auto pos_bal_a = b.pos_bal[0] - b.pos_bal[1];
    auto phase_a   = b.phase;
    auto hmc_a     = b.hmc;
    move(b, m);
    if (is_legal(b)) {
      if (pc) {
        cnt_mv_t(m, pc);
      }
      nodes += movegen_perft(b, depth - 1, pc);
    }
    unmove(b, m);
    auto hashb     = b.t_hash;
    auto m_hash_b  = b.m_hash;
    auto mat_bal_b = b.mat_bal;
    auto pos_bal_b = b.pos_bal[0] - b.pos_bal[1];
    auto phase_b   = b.phase;
    auto hmc_b     = b.hmc;
    assert(hasha == hashb);         // verify move unmove position hash
    assert(m_hash_a == m_hash_b);   // verify move unmove material hash
    assert(mat_bal_a == mat_bal_b); // verify move unmove material balance
    assert(pos_bal_a == pos_bal_b); // verify move unmove position balance
    assert(phase_a == phase_b);     // verify move unmove position balance
    assert(hmc_a == hmc_b);         // verify move unmove hcm
  }
  return nodes;
}

constexpr sz_t movegen_perft_nullmove_test(Board &b, const int depth,
                                           PerftCounts *pc = nullptr)
{
  MoveList ml;
  sz_t     nodes{};
  if (depth == 0) {
    return 1ULL;
  }
  movegen(b, ml.begin());
  sz_t       i   = 0;
  const sz_t nme = 3; /// null move every x moves
  for (const auto &m : ml) {
    if (m.from_sq == null_square) {
      break;
    }

    auto       hasha     = b.t_hash;
    auto       m_hash_a  = b.m_hash;
    auto       mat_bal_a = b.mat_bal;
    auto       pos_bal_a = b.pos_bal[0] - b.pos_bal[1];
    auto       phase_a   = b.phase;
    auto       hmc_a     = b.hmc;
    const auto nm        = Move{.prev_ep = b.ep};
    if (i % nme == 0) {
      move(b, nm);
    }
    else {
      move(b, m);
    }
    if (is_legal(b) && i % nme != 0) {
      if (pc) {
        cnt_mv_t(m, pc);
      }
      nodes += movegen_perft(b, depth - 1, pc);
    }
    if (i % nme == 0) {
      unmove(b, nm);
    }
    else {
      unmove(b, m);
    }
    auto hashb     = b.t_hash;
    auto m_hash_b  = b.m_hash;
    auto mat_bal_b = b.mat_bal;
    auto pos_bal_b = b.pos_bal[0] - b.pos_bal[1];
    auto phase_b   = b.phase;
    auto hmc_b     = b.hmc;
    assert(hasha == hashb);         // verify move unmove position hash
    assert(m_hash_a == m_hash_b);   // verify move unmove material hash
    assert(mat_bal_a == mat_bal_b); // verify move unmove material balance
    assert(pos_bal_a == pos_bal_b); // verify move unmove position balance
    assert(phase_a == phase_b);     // verify move unmove position balance
    assert(hmc_a == hmc_b);         // verify move unmove hcm
    ++i;
  }
  return nodes;
}

void print_res(const PerftCounts *pc)
{
  std::cout << "captures " << pc->captures << '\n';
  std::cout << "ep cap   " << pc->ep << '\n';
  std::cout << "castles  " << pc->castles << '\n';
  std::cout << "prom     " << pc->promotions << '\n';
}

namespace n_movegen_perft {

U64    total_nodes{};
sctime start;
sctime end;
} // namespace n_movegen_perft

TEST_CASE("nps")
{
  using namespace n_movegen_perft;
  std::array board{
      Board{},
      Board{
          "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - "},
      Board{"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 "},
      Board{"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"},
      Board{"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  "},
      Board{"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w "},
  };

  n_movegen_perft::start = std::chrono::steady_clock::now();
  total_nodes += movegen_perft(board[0], 5);
  total_nodes += movegen_perft(board[1], 3);
  total_nodes += movegen_perft(board[2], 4);
  total_nodes += movegen_perft(board[3], 4);
  total_nodes += movegen_perft(board[4], 4);
  total_nodes += movegen_perft(board[5], 4);
  end = std::chrono::steady_clock::now();

  std::cout << "nps: "
            << total_nodes /
                   std::chrono::duration_cast<std::chrono::milliseconds>(
                       end - n_movegen_perft::start)
                       .count() *
                   1000
            << '\n';
}

TEST_CASE("correctness")
{
  SECTION("perft initial")
  {
    Board b0;
    SECTION("depth 5") { CHECK(movegen_perft(b0, 5) == perft_results[1][5]); }
  }
  SECTION("perft position 2")
  {
    Board b0(
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
    SECTION("depth 3") { CHECK(movegen_perft(b0, 3) == perft_results[2][3]); }
  }
  SECTION("perft position 3")
  {
    Board b0("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 ");
    SECTION("depth 4") { CHECK(movegen_perft(b0, 4) == perft_results[3][4]); }
  }
  SECTION("perft position 4")
  {
    Board b0(
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    SECTION("depth 4") { CHECK(movegen_perft(b0, 4) == perft_results[4][4]); }
  }
  SECTION("perft position 5")
  {
    Board b0("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ");
    SECTION("depth 4") { CHECK(movegen_perft(b0, 4) == perft_results[5][4]); }
  }
  SECTION("perft position 6")
  {
    Board b0("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w "
             "- - 0 10 ");
    SECTION("depth 4") { CHECK(movegen_perft(b0, 4) == perft_results[6][4]); }
  }
}

TEST_CASE("null move")
{
  Board b0("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w "
           "- - 0 10 ");
  SECTION("depth 4") { CHECK(movegen_perft(b0, 4) == perft_results[6][4]); }
}
