cmake_minimum_required(VERSION 3.13)
enable_testing()

if(NOT TARGET pxart::pxart)
  find_package(pxart REQUIRED)
endif()

add_executable(main_tests main.cpp)
target_link_libraries(main_tests PRIVATE pxart::pxart)

add_test(main_tests main_tests)