find_package(fmt REQUIRED)
find_package(doctest REQUIRED)

add_library(librng INTERFACE)
add_executable(internal-tests)

# librng library
add_subdirectory(test)

target_sources(librng
  INTERFACE
    middle_square_engine.hpp
    middle_square_weyl_engine.hpp
    uniform_real_distribution.hpp
    xoroshiro128plus.hpp
)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  target_sources(librng
    INTERFACE
      rdrand_engine.hpp
  )
  target_compile_options(librng
    INTERFACE
      -mrdrnd
  )
endif()

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
target_sources(internal-tests
  PRIVATE
    main.test.cpp
    uniform_real_distribution.test.cpp
)

target_link_libraries(internal-tests
  PRIVATE
    doctest::doctest
    librng::librng
)

add_test(internal-tests internal-tests)