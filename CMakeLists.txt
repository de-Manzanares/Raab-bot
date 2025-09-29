cmake_minimum_required(VERSION 4.0)

project(Raab-bot LANGUAGES CXX)
set(VERSION v2-dev.5.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CompilerOptions -Wall -Wextra -Wpedantic
        # -Werror
)
set(DEBUGCompilerOptions -g ${CompilerOptions})
set(COVCompilerOptions --coverage ${CompilerOptions})
set(COVLinkerOptions --coverage)
set(SANCompilerOptions -fsanitize=address,undefined ${CompilerOptions})
set(SANLinkerOptions -fsanitize=address,undefined)
set(ReleaseCompilerOptions -O3 -ffast-math -march=native)

add_library(build_flags INTERFACE)
target_compile_options(build_flags
        INTERFACE
        $<$<CONFIG:Debug>:$<$<CXX_COMPILER_ID:GNU,Clang>:${DEBUGCompilerOptions}>>
        $<$<CONFIG:Coverage>:$<$<CXX_COMPILER_ID:GNU,Clang>:${COVCompilerOptions}>>
        $<$<CONFIG:Sanitizer>:$<$<CXX_COMPILER_ID:GNU,Clang>:${SANCompilerOptions}>>
        $<$<CONFIG:Release>:$<$<CXX_COMPILER_ID:GNU,Clang>:${ReleaseCompilerOptions}>>
)
target_link_options(build_flags
        INTERFACE
        $<$<CONFIG:Coverage>:$<$<CXX_COMPILER_ID:GNU,Clang>:${COVLinkerOptions}>>
        $<$<CONFIG:Sanitizer>:$<$<CXX_COMPILER_ID:GNU,Clang>:${SANLinkerOptions}>>
)

add_library(chess_modules OBJECT)
target_sources(chess_modules
        PUBLIC
        FILE_SET cxx_modules
        TYPE CXX_MODULES
        FILES
        modules/attack.ixx
        modules/board.ixx
        modules/eval.ixx
        modules/fen.ixx
        modules/move.ixx
        modules/movegen.ixx
        modules/search.ixx
        modules/transposition.ixx
        modules/types.ixx
        modules/uci.ixx
        modules/auxiliary/log.ixx
)
target_link_libraries(chess_modules PUBLIC build_flags)

add_library(chess STATIC)
target_sources(chess
        PRIVATE
        modules/details/attack.cpp
        modules/details/board.cpp
        modules/details/board.cpp
        modules/details/eval.cpp
        modules/details/fen.cpp
        modules/details/move.cpp
        modules/details/movegen.cpp
        modules/details/search.cpp
        modules/details/transposition.cpp
        modules/details/uci.cpp
        modules/auxiliary/log.cpp
)
target_link_libraries(chess PUBLIC chess_modules build_flags)

add_executable(Raab-bot-${VERSION} src/raab-bot.cpp)
target_link_libraries(Raab-bot-${VERSION} PRIVATE chess)

include(CTest)
enable_testing()

add_subdirectory(test)

add_subdirectory(tools)