set(CMAKE_CXX_STANDARD 23)

find_package(Catch2 3 QUIET)

if (Catch2_FOUND)
    message(STATUS "Catch2 v3 found, tests will be built")
    add_executable(basic-tests
            ../src/Square.cpp
            ../src/Game_State.cpp
            ../src/Board.cpp
            basic/general.cxx
            basic/influence-test.cxx
            basic/basic-moves-test.cxx
            basic/move-block-or-capture.cxx
            basic/pinned-pieces-test.cxx
            basic/sample-game.cxx)
    target_include_directories(basic-tests PRIVATE ../include)
    target_link_libraries(basic-tests PRIVATE Catch2::Catch2WithMain)

    add_executable(system-test
            ../src/Square.cpp
            ../src/Game_State.cpp
            ../src/Board.cpp
            ../src/Eval.cpp
            ../src/Node.cpp
            ../src/Search.cpp
            ../src/UCI.cpp
            other/system-test.cxx)
    target_include_directories(system-test PRIVATE ../include)
    target_link_libraries(system-test PRIVATE Catch2::Catch2WithMain)

    include(CTest)
    include(Catch)

    add_test(NAME BasicTest COMMAND basic-tests)
    add_test(NAME SystemTest COMMAND system-test)

else ()
    message(STATUS "Catch2 v3 not found, tests will not be built.")
endif ()