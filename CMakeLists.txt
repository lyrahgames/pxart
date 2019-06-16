cmake_minimum_required(VERSION 3.13)
project(pxart
  VERSION 0.1.0
  DESCRIPTION
    "pXart: Packed Extensions for Advanced Random Techniques - Header-only library for fast random number generators"
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

add_subdirectory(extern)
add_subdirectory(pxart)
add_subdirectory(rng)
add_subdirectory(testu01_utils)
add_subdirectory(tests)
add_subdirectory(bench)
