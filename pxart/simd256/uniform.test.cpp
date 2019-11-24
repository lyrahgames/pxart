#include <doctest/doctest.h>

#include <pxart/simd256/mt19937.hpp>
#include <pxart/simd256/uniform.hpp>
#include <pxart/uniform.hpp>
#include <random>

using namespace std;

TEST_CASE("pxart::simd256::detail::uniform") {
  pxart::simd256::mt19937 rng{random_device{}};
  const int n = 10'000'000;
  for (int i = 0; i < n; i += 8) {
    const auto rnd = rng();
    const auto v = pxart::simd256::detail::uniform<float>(rnd);
    for (int j = 0; j < 8; ++j) {
      REQUIRE(pxart::detail::uniform<float>(reinterpret_cast<const uint32_t*>(
                  &rnd)[j]) == reinterpret_cast<const float*>(&v)[j]);
    }
  }
  for (int i = 0; i < n; i += 4) {
    const auto rnd = rng();
    const auto v = pxart::simd256::detail::uniform<double>(rnd);
    for (int j = 0; j < 4; ++j) {
      REQUIRE(pxart::detail::uniform<double>(reinterpret_cast<const uint64_t*>(
                  &rnd)[j]) == reinterpret_cast<const double*>(&v)[j]);
    }
  }
}

TEST_CASE("pxart::simd256::detail::uniform Bounds") {
  pxart::simd256::mt19937 rng{random_device{}};
  const int n = 10'000'000;
  for (int i = 0; i < n; i += 8) {
    const auto rnd = rng();
    const auto v = pxart::simd256::detail::uniform(rnd, 0.125f, 0.83f);
    for (int j = 0; j < 8; ++j) {
      REQUIRE(pxart::detail::uniform(reinterpret_cast<const uint32_t*>(&rnd)[j],
                                     0.125f, 0.83f) ==
              reinterpret_cast<const float*>(&v)[j]);
    }
  }
  for (int i = 0; i < n; i += 4) {
    const auto rnd = rng();
    const auto v = pxart::simd256::detail::uniform(rnd, 0.11, 0.76);
    for (int j = 0; j < 4; ++j) {
      REQUIRE(pxart::detail::uniform(reinterpret_cast<const uint64_t*>(&rnd)[j],
                                     0.11, 0.76) ==
              reinterpret_cast<const double*>(&v)[j]);
    }
  }
}