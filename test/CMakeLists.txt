cmake_minimum_required(VERSION 3.28)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 17)

add_executable(general-test general.cxx)
add_executable(movement-test movement.cxx)
target_link_libraries(general-test PUBLIC
        /usr/local/lib/libCatch2Maind.a
        /usr/local/lib/libCatch2d.a)
target_link_libraries(movement-test PUBLIC
        /usr/local/lib/libCatch2Maind.a
        /usr/local/lib/libCatch2d.a)