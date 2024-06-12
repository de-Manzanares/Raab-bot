#include <catch2/catch_all.hpp>
#include <iostream>
#include "../../include/Board.h"

void generate_and_sort_ppt(Board& board, const Square& sq, std::vector<Square>& v)
{
    v.clear();
    board.update_move_maps();
    Color c = board.what_color(sq);
    if (c == Color::white) { v = board.maps->move_map_white[sq]; }
    if (c == Color::black) { v = board.maps->move_map_black[sq]; }
    std::sort(v.begin(), v.end());
}

using s = Square;

Board board_ppt;
std::vector<Square> v_ppt;

TEST_CASE("pinned pieces rook")
{
    board_ppt.import_fen("8/8/8/1kppR2K/8/8/8/8 w - - 0 1");        // blocked
    CHECK(board_ppt.pinned_piece_rook(s::e5) == s::e5);
    board_ppt.import_fen("8/8/8/1k1rPP1K/8/8/8/8 w - - 0 1");
    CHECK(board_ppt.pinned_piece_rook(s::d5) == s::d5);
    board_ppt.import_fen("8/3K4/8/3R4/3p4/3P4/3k4/8 w - - 0 1");
    CHECK(board_ppt.pinned_piece_rook(s::d5) == s::d5);
    board_ppt.import_fen("8/3K4/3p4/3P4/3r4/8/3k4/8 w - - 0 1");
    CHECK(board_ppt.pinned_piece_rook(s::d4) == s::d4);

    board_ppt.import_fen("8/8/8/1k1pR2K/8/8/8/8 w - - 0 1");        // pinned
    CHECK(board_ppt.pinned_piece_rook(s::e5) == s::d5);
    board_ppt.import_fen("8/8/8/1k1rP2K/8/8/8/8 w - - 0 1");
    CHECK(board_ppt.pinned_piece_rook(s::d5) == s::e5);
    board_ppt.import_fen("8/3K4/8/3R4/3p4/8/3k4/8 w - - 0 1");
    CHECK(board_ppt.pinned_piece_rook(s::d5) == s::d4);
    board_ppt.import_fen("8/3K4/8/3P4/3r4/8/3k4/8 w - - 0 1");
    CHECK(board_ppt.pinned_piece_rook(s::d4) == s::d5);
}

TEST_CASE("pinned pieces bishop")
{
    board_ppt.import_fen("6k1/8/5p2/8/8/2B5/1K6/8 w - - 0 1");      // not pinned
    CHECK(board_ppt.pinned_piece_bishop(s::c3) == s::c3);
    board_ppt.import_fen("8/6k1/5b2/8/8/2P5/K7/8 w - - 0 1");
    CHECK(board_ppt.pinned_piece_bishop(s::f6) == s::f6);
    board_ppt.import_fen("8/1K6/2B5/8/8/5p1k/8/8 w - - 0 1");
    CHECK(board_ppt.pinned_piece_bishop(s::c6) == s::c6);
    board_ppt.import_fen("8/K7/2P5/8/8/5b2/6k1/8 w - - 0 1");
    CHECK(board_ppt.pinned_piece_bishop(s::f3) == s::f3);

    board_ppt.import_fen("8/6k1/5p2/8/8/2B5/1K6/8 w - - 0 1");      // pinned
    CHECK(board_ppt.pinned_piece_bishop(s::c3) == s::f6);
    board_ppt.import_fen("8/6k1/5b2/8/8/2P5/1K6/8 w - - 0 1");
    CHECK(board_ppt.pinned_piece_bishop(s::f6) == s::c3);
    board_ppt.import_fen("8/1K6/2B5/8/8/5p2/6k1/8 w - - 0 1");
    CHECK(board_ppt.pinned_piece_bishop(s::c6) == s::f3);
    board_ppt.import_fen("8/1K6/2P5/8/8/5b2/6k1/8 w - - 0 1");
    CHECK(board_ppt.pinned_piece_bishop(s::f3) == s::c6);

}

TEST_CASE("lanes rook")
{
    board_ppt.import_fen("8/8/8/k2r1R1K/8/8/8/8 w - - 0 1");        // pinned
    generate_and_sort_ppt(board_ppt, s::d5, v_ppt);
    CHECK(v_ppt == std::vector<Square>{s::f5, s::e5, s::c5, s::b5});
    board_ppt.import_fen("8/8/8/kr4QK/8/8/8/8 w - - 0 1");
    generate_and_sort_ppt(board_ppt, s::g5, v_ppt);
    CHECK(v_ppt == std::vector<Square>{s::f5, s::e5, s::d5, s::c5, s::b5});
    board_ppt.import_fen("8/3K4/8/3R4/3b4/8/3k4/8 w - - 0 1");
    generate_and_sort_ppt(board_ppt, s::d4, v_ppt);
    CHECK(v_ppt == std::vector<Square>{});
    board_ppt.import_fen("8/3K4/8/3N4/3r4/8/3k4/8 w - - 0 1");
    generate_and_sort_ppt(board_ppt, s::d5, v_ppt);
    CHECK(v_ppt == std::vector<Square>{});
}

TEST_CASE("lanes bishop")
{
    board_ppt.import_fen("8/6k1/5b2/8/8/2B5/1K6/8 w - - 0 1");      // pinned
    generate_and_sort_ppt(board_ppt, s::f6, v_ppt);
    CHECK(v_ppt == std::vector<Square>{s::c3, s::d4, s::e5});
    board_ppt.import_fen("8/6k1/5b2/8/8/2Q5/1K6/8 w - - 0 1");
    generate_and_sort_ppt(board_ppt, s::c3, v_ppt);
    CHECK(v_ppt == std::vector<Square>{s::d4, s::e5, s::f6});
    board_ppt.import_fen("8/1K6/2B5/8/8/5n2/6k1/8 w - - 0 1");
    generate_and_sort_ppt(board_ppt, s::f3, v_ppt);
    CHECK(v_ppt == std::vector<Square>{});
    board_ppt.import_fen("8/1K6/2P5/8/8/5q2/6k1/8 w - - 0 1");
    generate_and_sort_ppt(board_ppt, s::c6, v_ppt);
    CHECK(v_ppt == std::vector<Square>{});
}