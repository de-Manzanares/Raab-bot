set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
add_executable(depths depths.cpp)
target_link_libraries(depths PRIVATE build_flags)