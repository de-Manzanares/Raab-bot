#ifndef CHESSENGINE_BOUNDARY_DETECTION_HPP
#define CHESSENGINE_BOUNDARY_DETECTION_HPP

#include "Square.hpp"

bool is_upper_vertical_boundary(Square square)
{
    return static_cast<int>(square) > 55;
}

bool is_lower_vertical_boundary(Square square)
{
    return static_cast<int>(square) < 8;
}

bool is_vertical_boundary(Square sq)
{
    return is_upper_vertical_boundary(sq) || is_lower_vertical_boundary(sq);
}

bool is_left_horizontal_boundary(Square sq)
{
    using s = Square;
    return sq == s::a1 || sq == s::a2 || sq == s::a3 || sq == s::a4 || sq == s::a5 || sq == s::a6 || sq == s::a7
            || sq == s::a8;
}

bool is_right_horizontal_boundary(Square sq)
{
    using s = Square;
    return sq == s::h1 || sq == s::h2 || sq == s::h3 || sq == s::h4 || sq == s::h5 || sq == s::h6 || sq == s::h7
            || sq == s::h8;
}

bool is_horizontal_boundary(Square sq)
{
    return is_left_horizontal_boundary(sq) || is_right_horizontal_boundary(sq);
}

bool is_boundary(Square sq)
{
    return is_vertical_boundary(sq) || is_horizontal_boundary(sq);
}

bool is_corner(Square sq)
{
    return is_vertical_boundary(sq) && is_horizontal_boundary(sq);
}

#endif //CHESSENGINE_BOUNDARY_DETECTION_HPP
