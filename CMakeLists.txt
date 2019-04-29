cmake_minimum_required(VERSION 3.13)
project(random-number-generators VERSION 0.1.0 LANGUAGES CXX C)
enable_testing()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

add_subdirectory(extern)
add_subdirectory(librng)
add_subdirectory(rng)
add_subdirectory(bench)