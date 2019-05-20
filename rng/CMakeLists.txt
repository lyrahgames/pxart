find_package(fmt REQUIRED)

add_executable(random_device_info)
target_sources(random_device_info
  PRIVATE
    random_device_info.cpp
)
target_link_libraries(random_device_info
  PRIVATE
    fmt::fmt
    pxart::pxart
)

add_executable(middle_square)
target_sources(middle_square
  PRIVATE
    middle_square.cpp
)
target_link_libraries(middle_square
  PRIVATE
    fmt::fmt
    pxart::pxart
)

add_executable(middle_square_weyl)
target_sources(middle_square_weyl
  PRIVATE
    middle_square_weyl.cpp
)
target_link_libraries(middle_square_weyl
  PRIVATE
    fmt::fmt
    pxart::pxart
)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  add_executable(rdrand)
  target_sources(rdrand
    PRIVATE
      rdrand.cpp
  )
  target_link_libraries(rdrand
    PRIVATE
      fmt::fmt
      pxart::pxart
  )
endif()

find_package(Threads)

add_executable(std_mt19937 std_mt19937.cpp)
target_compile_features(std_mt19937 PRIVATE cxx_std_17)
target_link_libraries(std_mt19937 PRIVATE Threads::Threads)

add_executable(std_random_device std_random_device.cpp)
target_compile_features(std_random_device PRIVATE cxx_std_17)
target_link_libraries(std_random_device PRIVATE Threads::Threads)

add_executable(simd_test simd_test.cpp)
target_compile_options(simd_test PRIVATE -mavx2)

find_package(TestU01 REQUIRED)

add_executable(mws mws.cpp)
target_link_libraries(mws
  PRIVATE
    pxart::pxart
    TestU01::TestU01
)

add_executable(xoroshiro128plus xoroshiro128plus.cpp)
target_link_libraries(xoroshiro128plus
  PRIVATE
    pxart::pxart
    TestU01::TestU01
)