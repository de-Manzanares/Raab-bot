cmake_minimum_required(VERSION 3.20)

project(Raab-bot)
add_subdirectory(test)

set(CMAKE_CXX_STANDARD 23)

add_executable(Raab-bot
        main.cpp
        src/Board.cpp
        src/Eval.cpp
        src/Game_State.cpp
        src/Node.cpp
        src/Search.cpp
        src/Square.cpp
        src/UCI.cpp)
target_include_directories(Raab-bot PRIVATE include)
