cmake_minimum_required(VERSION 4.3)
project(ptl LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

add_library(ptl INTERFACE)
target_sources(ptl
    INTERFACE
    FILE_SET HEADERS
    FILES
    ptl.hpp
)

target_include_directories(ptl
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
