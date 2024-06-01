cmake_minimum_required(VERSION 3.28)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 17)

add_executable(general-test general.cxx)

add_executable(influence-test influence-test.cxx)

add_executable(legal-moves-test legal-moves-test.cxx)

target_link_libraries(general-test PUBLIC
        /usr/local/lib/libCatch2Maind.a
        /usr/local/lib/libCatch2d.a)

target_link_libraries(influence-test PUBLIC
        /usr/local/lib/libCatch2Maind.a
        /usr/local/lib/libCatch2d.a)

target_link_libraries(legal-moves-test PUBLIC
        /usr/local/lib/libCatch2Maind.a
        /usr/local/lib/libCatch2d.a)
