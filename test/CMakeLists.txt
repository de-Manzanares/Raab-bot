include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.10.0
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Catch2)

add_library(all_modules OBJECT)
target_sources(all_modules PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES src/all_modules.ixx)
target_link_libraries(all_modules PUBLIC chess)

add_executable(attack-test src/attack.cxx)
target_link_libraries(attack-test PRIVATE all_modules Catch2::Catch2WithMain)
add_test(NAME AttackTest COMMAND attack-test)

add_executable(movegen-simple-test src/movegen_simple.cxx)
target_link_libraries(movegen-simple-test PRIVATE all_modules Catch2::Catch2WithMain)
add_test(NAME MovegenSimpleTest COMMAND movegen-simple-test)

add_executable(movegen-perft-test src/movegen_perft.cxx)
target_link_libraries(movegen-perft-test PRIVATE all_modules Catch2::Catch2WithMain)
add_test(NAME MovegenPerftTest COMMAND movegen-perft-test)

add_executable(search-test src/search.cxx)
target_link_libraries(search-test PRIVATE all_modules Catch2::Catch2WithMain)
add_test(NAME SearchTest COMMAND search-test)
