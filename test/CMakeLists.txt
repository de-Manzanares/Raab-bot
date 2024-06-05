cmake_minimum_required(VERSION 3.28)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 17)

add_executable(general-test general.cxx)

add_executable(influence-test influence-test.cxx)

add_executable(basic-moves-test basic-moves-test.cxx)

add_executable(move-block-or-capture-test move-block-or-capture.cxx)

add_executable(other-moves-test other-moves-test.cxx)

add_executable(pinned-pieces-test pinned-pieces-test.cxx)

add_executable(scratch-test scratch.cxx)

add_executable(evaluation-test evaluation-test.cxx)

add_executable(search-test search-test.cxx)

add_executable(uci-test uci-test.cxx)

target_link_libraries(general-test PUBLIC
        /usr/local/lib/libCatch2Maind.a
        /usr/local/lib/libCatch2d.a)

target_link_libraries(influence-test PUBLIC
        /usr/local/lib/libCatch2Maind.a
        /usr/local/lib/libCatch2d.a)

target_link_libraries(basic-moves-test PUBLIC
        /usr/local/lib/libCatch2Maind.a
        /usr/local/lib/libCatch2d.a)

target_link_libraries(move-block-or-capture-test PUBLIC
        /usr/local/lib/libCatch2Maind.a
        /usr/local/lib/libCatch2d.a)

target_link_libraries(other-moves-test PUBLIC
        /usr/local/lib/libCatch2Maind.a
        /usr/local/lib/libCatch2d.a)

target_link_libraries(pinned-pieces-test PUBLIC
        /usr/local/lib/libCatch2Maind.a
        /usr/local/lib/libCatch2d.a)

target_link_libraries(scratch-test PUBLIC
        /usr/local/lib/libCatch2Maind.a
        /usr/local/lib/libCatch2d.a)

target_link_libraries(evaluation-test PUBLIC
        /usr/local/lib/libCatch2Maind.a
        /usr/local/lib/libCatch2d.a)

target_link_libraries(search-test PUBLIC
        /usr/local/lib/libCatch2Maind.a
        /usr/local/lib/libCatch2d.a)

target_link_libraries(uci-test PUBLIC
        /usr/local/lib/libCatch2Maind.a
        /usr/local/lib/libCatch2d.a)
