set(CMAKE_CXX_STANDARD 23)

find_package(Catch2 3 QUIET)

if (Catch2_FOUND)
    message(STATUS "Catch2 v3 found, tests will be built")
    add_executable(board-test
            ../src/Square.cpp
            ../src/Game_State.cpp
            ../src/Board.cpp
            board/general.cxx
            board/influence-test.cxx
            board/basic-moves-test.cxx
            board/move-block-or-capture.cxx
            board/pinned-pieces-test.cxx
            board/sample-game.cxx)
    target_include_directories(board-test PRIVATE ../include)
    target_link_libraries(board-test PRIVATE Catch2::Catch2WithMain)

    add_executable(other-test
            ../src/Square.cpp
            ../src/Game_State.cpp
            ../src/Board.cpp
            ../src/Eval.cpp
            ../src/Node.cpp
            ../src/Search.cpp
            ../src/UCI.cpp
            other/uci-test.cxx
            other/stalemate-test.cxx)
    target_include_directories(other-test PRIVATE ../include)
    target_link_libraries(other-test PRIVATE Catch2::Catch2WithMain)

    include(CTest)
    include(Catch)

    add_test(NAME BoardTest COMMAND board-test)
    add_test(NAME OtherTest COMMAND other-test)

else ()
    message(STATUS "Catch2 v3 not found, tests will not be built.")
endif ()