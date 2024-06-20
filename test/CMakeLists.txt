cmake_minimum_required(VERSION 3.28)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 17)

add_executable(basic-tests ../src/Square.cpp ../src/Game_State.cpp ../src/Board.cpp basic/general.cxx basic/influence-test.cxx
        basic/basic-moves-test.cxx basic/move-block-or-capture.cxx basic/pinned-pieces-test.cxx basic/sample-game.cxx)

add_executable(system-test ../src/Square.cpp ../src/Game_State.cpp ../src/Board.cpp ../src/Eval.cpp ../src/Node.cpp
        ../src/Search.cpp other/system-test.cxx)

target_link_libraries(basic-tests PUBLIC /usr/local/lib/libCatch2Maind.a /usr/local/lib/libCatch2d.a)

target_link_libraries(system-test PUBLIC /usr/local/lib/libCatch2Maind.a /usr/local/lib/libCatch2d.a)
