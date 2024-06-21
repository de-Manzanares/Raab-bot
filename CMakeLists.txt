cmake_minimum_required(VERSION 3.28)

project(Raab-bot)

set(CMAKE_CXX_STANDARD 17)

add_executable(v24.06.21 main.cpp
        src/Board.cpp src/Eval.cpp src/Game_State.cpp src/Node.cpp src/Search.cpp src/Square.cpp src/UCI.cpp)
