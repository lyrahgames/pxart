#include <doctest/doctest.h>

#include <pxart/simd128/mt19937.hpp>
#include <random>

#ifndef PXART_SUPPORT_SIMD128_MT19937

TEST_CASE("pxart::simd128::mt19937") {
  MESSAGE("Could not be tested. Not supported.");
}

#else  // PXART_SUPPORT_SIMD128_MT19937

TEST_CASE("pxart::simd128::mt19937 Default Initialization") {
  std::mt19937 rng{};
  pxart::simd128::mt19937 vrng{};
  const int step = pxart::simd128::mt19937::simd_size;

  const int n = 10'000'000;
  for (int i = 0; i < n; i += step) {
    const auto v128 = vrng();
    for (int j = 0; j < step; ++j) {
      const auto rnd = rng();
      const auto vrnd = reinterpret_cast<const uint32_t*>(&v128)[j];
      REQUIRE(rnd == vrnd);
    }
  }
}

TEST_CASE("pxart::simd128::mt19937 Random Initialization with Default Seeder") {
  const auto seed = std::random_device{}();
  std::mt19937 rng{seed};
  pxart::simd128::mt19937 vrng{pxart::mt19937::default_seeder{seed}};
  const int step = pxart::simd128::mt19937::simd_size;

  const int n = 10'000'000;
  for (int i = 0; i < n; i += step) {
    const auto v128 = vrng();
    for (int j = 0; j < step; ++j) {
      const auto rnd = rng();
      const auto vrnd = reinterpret_cast<const uint32_t*>(&v128)[j];
      REQUIRE(rnd == vrnd);
    }
  }
}

#endif  // PXART_SUPPORT_SIMD128_MT19937