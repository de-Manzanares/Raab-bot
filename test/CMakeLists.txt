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
        src/attack.cxx
)
target_link_libraries(
        attack-test
        PRIVATE
        common_options
        chess_modules
        Catch2::Catch2WithMain
)
add_test(
        NAME AttackTest
        COMMAND attack-test
)

add_executable(
        movegen-test
        src/movegen.cxx
)
target_link_libraries(
        movegen-test
        PRIVATE
        common_options
        chess_modules
        Catch2::Catch2WithMain
)
add_test(
        NAME MovegenTest
        COMMAND movegen-test
)
