cmake_minimum_required(VERSION 4.0)

################################################################################

project(Raab-bot LANGUAGES CXX)
set(VERSION v3-dev.0)
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

################################################################################

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

add_library(includes INTERFACE)
target_sources(includes PRIVATE
        include/Array.hpp
        include/defs.h
        include/Vector.hpp
)

################################################################################

add_executable(Raab-bot-${VERSION} src/raab-bot.cpp)
target_include_directories(Raab-bot-${VERSION} PRIVATE include)
target_link_libraries(Raab-bot-${VERSION} PRIVATE build_flags includes)

################################################################################

include(CTest)
enable_testing()

add_subdirectory(test)
