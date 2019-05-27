# if(NOT TARGET pxart::pxart)
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
  cmake_minimum_required(VERSION 3.13)
  message(STATUS "Tests for 'pXart' will be built separately.")
  enable_testing()
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../cmake")
  find_package(pxart REQUIRED)
endif()

add_executable(main_tests main.cpp)
target_link_libraries(main_tests PRIVATE pxart::pxart)

add_test(main_tests main_tests)

find_package(TestU01 REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)

add_library(testu01_utils
  testu01_utils.hpp
  testu01_utils.cpp
)
target_include_directories(testu01_utils PUBLIC .)
target_link_libraries(testu01_utils
  PUBLIC
    Boost::boost
  PRIVATE
    TestU01::TestU01
)
target_compile_features(testu01_utils PRIVATE cxx_std_17)

add_executable(testu01_middle_square_weyl
  testu01_middle_square_weyl.cpp
)
target_link_libraries(testu01_middle_square_weyl
  PRIVATE
    pxart::pxart
    # TestU01::TestU01
    # Boost::boost
    Boost::program_options
    testu01_utils
)