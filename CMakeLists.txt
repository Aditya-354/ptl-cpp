cmake_minimum_required(VERSION 4.3)
project(ptl_test LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

add_executable(test)
target_sources(test
    PRIVATE
    src/linked_list_test.cpp
)

add_subdirectory(ptl)

target_link_libraries(test
    PUBLIC
    ptl
)

target_compile_options(test
    INTERFACE
    -Wall -Wextra -Werror Wpedantic
)
