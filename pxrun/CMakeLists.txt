find_package(Boost REQUIRED COMPONENTS program_options)

add_executable(pxrun
  main.cpp
  matrix.cpp
  testu01.cpp
)

target_include_directories(pxrun PRIVATE ..)

target_compile_features(pxrun PRIVATE cxx_std_17)

target_link_libraries(pxrun
  PRIVATE
    Boost::boost
    Boost::program_options
    pxart::pxart
    testu01_utils::testu01_utils
)