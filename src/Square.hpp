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
    h8, g8, f8, e8, d8, c8, b8, a8,
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

Square string_to_square(const std::string& string)
{
    Square square{};
    std::unordered_map<std::string, Square> stosq = {
            {"h1", Square::h1},
            {"g1", Square::g1},
            {"f1", Square::f1},
            {"e1", Square::e1},
            {"d1", Square::d1},
            {"c1", Square::c1},
            {"b1", Square::b1},
            {"a1", Square::a1},
            {"h2", Square::h2},
            {"g2", Square::g2},
            {"f2", Square::f2},
            {"e2", Square::e2},
            {"d2", Square::d2},
            {"c2", Square::c2},
            {"b2", Square::b2},
            {"a2", Square::a2},
            {"h3", Square::h3},
            {"g3", Square::g3},
            {"f3", Square::f3},
            {"e3", Square::e3},
            {"d3", Square::d3},
            {"c3", Square::c3},
            {"b3", Square::b3},
            {"a3", Square::a3},
            {"h4", Square::h4},
            {"g4", Square::g4},
            {"f4", Square::f4},
            {"e4", Square::e4},
            {"d4", Square::d4},
            {"c4", Square::c4},
            {"b4", Square::b4},
            {"a4", Square::a4},
            {"h5", Square::h5},
            {"g5", Square::g5},
            {"f5", Square::f5},
            {"e5", Square::e5},
            {"d5", Square::d5},
            {"c5", Square::c5},
            {"b5", Square::b5},
            {"a5", Square::a5},
            {"h6", Square::h6},
            {"g6", Square::g6},
            {"f6", Square::f6},
            {"e6", Square::e6},
            {"d6", Square::d6},
            {"c6", Square::c6},
            {"b6", Square::b6},
            {"a6", Square::a6},
            {"h7", Square::h7},
            {"g7", Square::g7},
            {"f7", Square::f7},
            {"e7", Square::e7},
            {"d7", Square::d7},
            {"c7", Square::c7},
            {"b7", Square::b7},
            {"a7", Square::a7},
            {"h8", Square::h8},
            {"g8", Square::g8},
            {"f8", Square::f8},
            {"e8", Square::e8},
            {"d8", Square::d8},
            {"c8", Square::c8},
            {"b8", Square::b8},
            {"a8", Square::a8}
    };
    for (const auto& [s, sq] : stosq) {
        if (string == s) {
            square = sq;
            break;
        }
    }
    return square;
}

// TODO overload stream extraction operator

#endif //CHESSENGINE_SQUARE_H
