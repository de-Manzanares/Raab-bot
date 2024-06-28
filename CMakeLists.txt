cmake_minimum_required(VERSION 3.20)

project(Raab-bot)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-Og")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto=auto -fuse-linker-plugin -march=native")
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(include)

add_executable(Raab-bot
        main.cpp
        src/Board.cpp
        src/Eval.cpp
        src/Game_State.cpp
        src/Node.cpp
        src/Search.cpp
        src/Square.cpp
        src/UCI.cpp)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()
