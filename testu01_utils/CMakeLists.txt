find_package(TestU01 REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)

add_library(testu01_utils
  testu01_utils.hpp
  testu01_utils.cpp
)
target_include_directories(testu01_utils PUBLIC ..)
target_link_libraries(testu01_utils
  PUBLIC
    Boost::boost
    pxart::pxart
  PRIVATE
    TestU01::TestU01
)
target_compile_features(testu01_utils PRIVATE cxx_std_17)
add_library(testu01_utils::testu01_utils ALIAS testu01_utils)

add_executable(testu01_test main.cpp)
target_link_libraries(testu01_test
  PRIVATE
    pxart::pxart
    Boost::boost
    Boost::program_options
    testu01_utils::testu01_utils
)