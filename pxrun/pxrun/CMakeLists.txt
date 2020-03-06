add_executable(pxrun
  main.cpp
  matrix.hpp
  matrix.cpp
  testu01.hpp
  testu01.cpp
  testu01_utils.hpp
  testu01_utils.cpp
)

target_include_directories(pxrun PRIVATE ..)

target_compile_features(pxrun PRIVATE cxx_std_17)

target_link_libraries(pxrun
  PRIVATE
    Boost::boost
    Boost::program_options
    pxart::pxart
    TestU01::TestU01
)