#include <doctest/doctest.h>

#include <pxart/simd256/mt19937.hpp>
#include <random>

#ifndef PXART_SUPPORT_SIMD256_MT19937

TEST_CASE("pxart::simd256::mt19937") {
  MESSAGE("Could not be tested. Not supported.");
}

#else  // PXART_SUPPORT_SIMD256_MT19937

TEST_CASE("pxart::simd256::mt19937 Default Initialization") {
  std::mt19937 rng{};
  pxart::simd256::mt19937 vrng{};

  const int n = 10'000'000;
  for (int i = 0; i < n; i += 8) {
    const auto v256 = vrng();
    for (int j = 0; j < 8; ++j) {
      const auto rnd = rng();
      const auto vrnd = reinterpret_cast<const uint32_t*>(&v256)[j];
      REQUIRE(rnd == vrnd);
    }
  }
}

TEST_CASE("pxart::simd256::mt19937 Random Initialization with Default Seeder") {
  const auto seed = std::random_device{}();
  std::mt19937 rng{seed};
  pxart::simd256::mt19937 vrng{pxart::mt19937::default_seeder{seed}};

  const int n = 10'000'000;
  for (int i = 0; i < n; i += 8) {
    const auto v256 = vrng();
    for (int j = 0; j < 8; ++j) {
      const auto rnd = rng();
      const auto vrnd = reinterpret_cast<const uint32_t*>(&v256)[j];
      REQUIRE(rnd == vrnd);
    }
  }
}

#endif  // PXART_SUPPORT_SIMD256_MT19937