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