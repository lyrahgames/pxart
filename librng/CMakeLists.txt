# librng library
find_package(fmt REQUIRED)
add_library(librng INTERFACE)
target_sources(librng
  INTERFACE
    sample_analyzer.hpp
)
target_include_directories(librng
  INTERFACE
    ..
)
target_compile_features(librng
  INTERFACE
    cxx_std_17
)
target_link_libraries(librng
  INTERFACE
    fmt::fmt
)
add_library(librng::librng ALIAS librng)

# internal tests
find_package(doctest REQUIRED)
add_executable(internal-tests)
target_sources(internal-tests
  PRIVATE
    main.test.cpp
)
target_link_libraries(internal-tests
  PRIVATE
    doctest::doctest
    librng::librng
)