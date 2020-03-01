#include <doctest/doctest.h>

#include <pxart/msws.hpp>
#include <pxart/simd128/msws.hpp>
#include <random>

#ifndef PXART_SUPPORT_SIMD128_MSWS

TEST_CASE("pxart::simd128::msws") {
  MESSAGE("Could not be tested. Not supported.");
}

#else  // PXART_SUPPORT_SIMD128_MSWS

TEST_CASE("pxart::simd128::msws Vectorization") {
  pxart::simd128::msws rng1{std::random_device{}};
  pxart::msws rng2[4]{};
  for (int i = 0; i < 4; ++i) {
    rng2[i].s = reinterpret_cast<const uint64_t*>(&rng1.step)[i];
  }

  for (int i = 0; i < 4; ++i) {
    CHECK(rng2[i].x == reinterpret_cast<const uint64_t*>(&rng1.root)[i]);
    CHECK(rng2[i].w == reinterpret_cast<const uint64_t*>(&rng1.weyl)[i]);
    CHECK(rng2[i].s == reinterpret_cast<const uint64_t*>(&rng1.step)[i]);
  }

  const int n = 10'000'000;
  for (int i = 0; i < n; i += 4) {
    const auto tmp = rng1();
    for (int j = 0; j < 4; ++j) {
      const auto rnd1 = reinterpret_cast<const uint32_t*>(&tmp)[j];
      const auto rnd2 = rng2[j]();
      CHECK(rnd1 == rnd2);
    }
  }
}

#endif  // PXART_SUPPORT_SIMD128_MSWS