cmake_minimum_required(VERSION 3.13)
enable_testing()

if(NOT TARGET pxart::pxart)
  find_package(pxart REQUIRED)
endif()

add_executable(main_tests main.cpp)
target_link_libraries(main_tests PRIVATE pxart::pxart)

add_test(main_tests main_tests)

find_package(TestU01 REQUIRED)
find_package(Boost REQUIRED)

add_executable(testu01_middle_square_weyl
  testu01_middle_square_weyl.cpp
)
target_link_libraries(testu01_middle_square_weyl
  PRIVATE
    pxart::pxart
    TestU01::TestU01
    Boost::boost
)