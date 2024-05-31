#ifndef CHESSENGINE_SQUARE_H
#define CHESSENGINE_SQUARE_H

// do not make underlying uint8_t
enum class Square : int {
    h1, g1, f1, e1, d1, c1, b1, a1,
    h2, g2, f2, e2, d2, c2, b2, a2,
    h3, g3, f3, e3, d3, c3, b3, a3,
    h4, g4, f4, e4, d4, c4, b4, a4,
    h5, g5, f5, e5, d5, c5, b5, a5,
    h6, g6, f6, e6, d6, c6, b6, a6,
    h7, g7, f7, e7, d7, c7, b7, a7,
    h8, g8, f8, e8, d8, c8, b8, a8
};

Square& operator--(Square& square)
{
    square = static_cast<Square>(static_cast<int>(square) - 1);
    return square;
}

Square& operator++(Square& square)
{
    square = static_cast<Square>(static_cast<int>(square) + 1);
    return square;
}

template<class T>
Square operator+(Square lhs, T rhs)
{
    static_assert(std::is_integral<T>::value);
    Square copy = lhs;
    for (auto i = 0; i < rhs; i++) {
        rhs > 0 ? ++copy : --copy;
    }
    return copy;
}

template<class T>
Square operator-(Square lhs, T rhs)
{
    static_assert(std::is_integral<T>::value);
    Square copy = lhs;
    for (auto i = 0; i < rhs; i++) {
        rhs > 0 ? --copy : ++copy;
    }
    return copy;
}

enum class Color {
    white, black, none
};

Color operator!(Color color)
{
    return color == Color::white ? Color::black : Color::white;
}

// TODO overload stream extraction operator

#endif //CHESSENGINE_SQUARE_H
