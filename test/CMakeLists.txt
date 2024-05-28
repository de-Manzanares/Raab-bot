cmake_minimum_required(VERSION 3.28)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 17)

add_executable(ChessEngine-test bitboard.cxx)
target_link_libraries(ChessEngine-test PUBLIC
        /usr/local/lib/libCatch2Maind.a
        /usr/local/lib/libCatch2d.a)