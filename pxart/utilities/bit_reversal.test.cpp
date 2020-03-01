#include <doctest/doctest.h>

#include <limits>
#include <pxart/utilities/bit_reversal.hpp>

using namespace pxart;

TEST_CASE("Bit Reversal for 8 bit int") {
  CHECK(bit_reversal<8>(0) == 0);
  CHECK(bit_reversal<8>(1) == 0b10000000);
  CHECK(bit_reversal<8>(2) == 0b01000000);
  CHECK(bit_reversal<8>(0b10011010) == 0b01011001);

  for (int i = 0; i < 256; ++i) CHECK(bit_reversal<8>(bit_reversal<8>(i)) == i);
}

TEST_CASE("Bit Reversal for 8 bit signed char") {
  for (signed char i = -128; i < 127; ++i)
    CHECK(bit_reversal<8>(bit_reversal<8>(i)) == i);
}

TEST_CASE("Bit Reversal for 16 bit int") {
  CHECK(bit_reversal<16>(0b1001111100101101) == 0b1011010011111001);

  for (int i = 0; i < (1 << 16); ++i)
    CHECK(bit_reversal<16>(bit_reversal<16>(i)) == i);
}

TEST_CASE("Bit Reversal for 32 bit int") {
  CHECK(bit_reversal<32>(0b10011101101001111011001010111010) ==
        0b01011101010011011110010110111001);
  for (int i = 0; i < std::numeric_limits<int>::max() - 10000; i += 1011)
    CHECK(bit_reversal<32>(bit_reversal<32>(i)) == i);
}

TEST_CASE("Bit Reversal for 64 bit long") {
  CHECK(
      bit_reversal<64>(
          0b1001110110100111101100101011101001011101010011011110010110111001L) ==
      0b1001110110100111101100101011101001011101010011011110010110111001L);
}

TEST_CASE("Bit Reversal for int") {
  CHECK(bit_reversal(0) == 0);
  CHECK(bit_reversal(-1) == -1);
  CHECK(bit_reversal(1) == std::numeric_limits<int>::min());
  CHECK(bit_reversal(bit_reversal(-2)) == -2);
}