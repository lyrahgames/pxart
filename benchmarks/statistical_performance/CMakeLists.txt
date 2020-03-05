add_executable(mt19937_bitstream mt19937_bitstream.cpp bitstream.ipp)
target_link_libraries(mt19937_bitstream PRIVATE pxart::pxart)

add_executable(mt19937_simd128_bitstream mt19937_simd128_bitstream.cpp bitstream.ipp)
target_link_libraries(mt19937_simd128_bitstream PRIVATE pxart::pxart)

add_executable(mt19937_simd256_bitstream mt19937_simd256_bitstream.cpp bitstream.ipp)
target_link_libraries(mt19937_simd256_bitstream PRIVATE pxart::pxart)

add_executable(mt19937_testu01 mt19937_testu01.cpp testu01.ipp)
target_link_libraries(mt19937_testu01 PRIVATE pxart::pxart TestU01::TestU01)
