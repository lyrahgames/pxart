#include <doctest/doctest.h>

#include <array>
#include <cstdint>
#include <pxart/utility/bool1x256.hpp>
#include <pxart/utility/pun_cast.hpp>
#include <type_traits>
#include <vector>

using namespace std;

#ifdef __AVX2__

TEST_CASE("pxart::simd256::bool1x256 default constructor") {
  using pxart::simd256::bool1x256;
  bool1x256 v{};
  const auto sv = pxart::pun_cast<array<uint32_t, 8>>(v);
  for (const auto x : sv) CHECK(x == 0);
}

#endif  // __AVX2__
