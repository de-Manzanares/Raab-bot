cmake_minimum_required(VERSION 3.28)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 17)

add_executable(general-test ../src/Square.cpp ../src/Game_State.cpp ../src/Board.cpp basic/general.cxx)

add_executable(influence-test ../src/Square.cpp ../src/Game_State.cpp ../src/Board.cpp basic/influence-test.cxx)

add_executable(basic-moves-test ../src/Square.cpp ../src/Game_State.cpp ../src/Board.cpp basic/basic-moves-test.cxx)

add_executable(move-block-or-capture-test ../src/Square.cpp ../src/Game_State.cpp ../src/Board.cpp basic/move-block-or-capture.cxx)

add_executable(pinned-pieces-test ../src/Square.cpp ../src/Game_State.cpp ../src/Board.cpp basic/pinned-pieces-test.cxx)

add_executable(sample-game-test ../src/Square.cpp ../src/Game_State.cpp ../src/Board.cpp basic/sample-game.cxx)

add_executable(other-moves-test ../src/Square.cpp ../src/Game_State.cpp ../src/Board.cpp other/other-moves-test.cxx)

add_executable(basic-tests ../src/Square.cpp ../src/Game_State.cpp ../src/Board.cpp basic/general.cxx basic/influence-test.cxx
        basic/basic-moves-test.cxx basic/move-block-or-capture.cxx basic/pinned-pieces-test.cxx basic/sample-game.cxx)

add_executable(evaluation-test ../src/Square.cpp ../src/Game_State.cpp ../src/Board.cpp ../src/Eval.cpp ../src/Node.cpp
        other/evaluation-test.cxx)

add_executable(search-test ../src/Square.cpp ../src/Game_State.cpp ../src/Board.cpp ../src/Eval.cpp ../src/Node.cpp
        ../src/Search.cpp other/search-test.cxx)

add_executable(uci-test ../src/Square.cpp ../src/Game_State.cpp ../src/Board.cpp ../src/Eval.cpp ../src/Node.cpp
        ../src/Search.cpp other/uci-test.cxx)


target_link_libraries(general-test PUBLIC /usr/local/lib/libCatch2Maind.a /usr/local/lib/libCatch2d.a)

target_link_libraries(influence-test PUBLIC /usr/local/lib/libCatch2Maind.a /usr/local/lib/libCatch2d.a)

target_link_libraries(basic-moves-test PUBLIC /usr/local/lib/libCatch2Maind.a /usr/local/lib/libCatch2d.a)

target_link_libraries(move-block-or-capture-test PUBLIC /usr/local/lib/libCatch2Maind.a /usr/local/lib/libCatch2d.a)

target_link_libraries(other-moves-test PUBLIC /usr/local/lib/libCatch2Maind.a /usr/local/lib/libCatch2d.a)

target_link_libraries(pinned-pieces-test PUBLIC /usr/local/lib/libCatch2Maind.a /usr/local/lib/libCatch2d.a)

target_link_libraries(sample-game-test PUBLIC /usr/local/lib/libCatch2Maind.a /usr/local/lib/libCatch2d.a)

target_link_libraries(basic-tests PUBLIC /usr/local/lib/libCatch2Maind.a /usr/local/lib/libCatch2d.a)

target_link_libraries(evaluation-test PUBLIC /usr/local/lib/libCatch2Maind.a /usr/local/lib/libCatch2d.a)

target_link_libraries(search-test PUBLIC /usr/local/lib/libCatch2Maind.a /usr/local/lib/libCatch2d.a)

target_link_libraries(uci-test PUBLIC /usr/local/lib/libCatch2Maind.a /usr/local/lib/libCatch2d.a)
