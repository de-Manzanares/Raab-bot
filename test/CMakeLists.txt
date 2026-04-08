include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.10.0
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Catch2)

add_executable(test0 src/test0.cxx)
target_include_directories(test0 PRIVATE ../include)
target_link_libraries(test0 PRIVATE build_flags Catch2::Catch2WithMain includes)
add_test(NAME PerfT COMMAND test0)

