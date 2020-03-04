# add_executable(driver driver.cpp)
# target_link_libraries(driver PRIVATE pxart::pxart)

add_executable(main_tests
  bit_reversal.cpp
  is_valid.cpp
  msws.cpp
  mt19937.cpp
  uniform.cpp
  xoroshiro128plus.cpp
  simd128/msws.cpp
  simd128/mt19937.cpp
  simd128/uniform.cpp
  simd128/xoroshiro128plus.cpp
  simd256/msws.cpp
  simd256/mt19937.cpp
  simd256/uniform.cpp
  simd256/xoroshiro128plus.cpp
)

target_link_libraries(main_tests
  PRIVATE
    doctest::doctest
    pxart::pxart
)

add_test(main_tests main_tests)