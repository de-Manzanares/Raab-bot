cmake_minimum_required(VERSION 4.0)

project(Raab-bot LANGUAGES CXX)
set(VERSION v2-dev)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CompilerOptions -Wall -Wextra -Wpedantic
        # -Werror
)
set(DEBUGCompilerOptions -g ${CompilerOptions})
set(COVCompilerOptions --coverage ${CompilerOptions})
set(COVLinkerOptions --coverage)
set(SANCompilerOptions -fsanitize=address,undefined ${CompilerOptions})
set(SANLinkerOptions -fsanitize=address,undefined)
set(ReleaseCompilerOptions -O3 -ffast-math -march=native)

add_library(
        common_options
        INTERFACE
)
target_compile_options(
        common_options
        INTERFACE
        $<$<CONFIG:Debug>:$<$<CXX_COMPILER_ID:GNU,Clang>:${DEBUGCompilerOptions}>>
        $<$<CONFIG:Coverage>:$<$<CXX_COMPILER_ID:GNU,Clang>:${COVCompilerOptions}>>
        $<$<CONFIG:Sanitizer>:$<$<CXX_COMPILER_ID:GNU,Clang>:${SANCompilerOptions}>>
        $<$<CONFIG:Release>:$<$<CXX_COMPILER_ID:GNU,Clang>:${ReleaseCompilerOptions}>>
)
target_link_options(
        common_options
        INTERFACE
        $<$<CONFIG:Coverage>:$<$<CXX_COMPILER_ID:GNU,Clang>:${COVLinkerOptions}>>
        $<$<CONFIG:Sanitizer>:$<$<CXX_COMPILER_ID:GNU,Clang>:${SANLinkerOptions}>>
)

add_library(
        chess_modules
        OBJECT
)
target_sources(
        chess_modules
        PUBLIC
        FILE_SET cxx_modules
        TYPE CXX_MODULES
        FILES
        src/attack.cppm
        src/Board0x88.cppm
        src/chess_types.cppm
        src/fen.cppm
        src/movegen.cppm
)
target_link_libraries(
        chess_modules
        PRIVATE
        common_options
)

add_executable(
        Raab-bot-${VERSION}
        src/main.cpp
)
target_link_libraries(
        Raab-bot-${VERSION}
        PRIVATE
        common_options
        chess_modules
)

enable_testing()
include(CTest)
include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.10.0
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Catch2)

add_executable(
        attack-test
        test/attack.cxx
)
target_link_libraries(
        attack-test
        PRIVATE
        common_options
        chess_modules
        Catch2::Catch2WithMain)
add_test(
        NAME AttackTest
        COMMAND attack-test
)
