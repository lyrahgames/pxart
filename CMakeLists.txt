cmake_minimum_required(VERSION 3.13)
project(pxart
  VERSION 0.1.0
  DESCRIPTION
    "pXart: Packed Extensions for Advanced Random Techniques - Header-only Library for Fast Pseudorandom Number Generators"
  HOMEPAGE_URL
    "https://github.com/lyrahgames/pxart"
  LANGUAGES CXX C
)
enable_testing()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(TestBigEndian)
test_big_endian(is_big_endian)
include(DefaultBuildType)
include(Uninstall)

add_subdirectory(pxart)
add_subdirectory(tests)
add_subdirectory(benchmarks)
