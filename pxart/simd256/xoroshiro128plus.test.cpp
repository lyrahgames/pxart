#include <doctest/doctest.h>

#include <pxart/simd256/xoroshiro128plus.hpp>
#include <pxart/xoroshiro128plus.hpp>
#include <random>

namespace {
inline bool equal(__m256i x, __m256i y) noexcept {
  for (int i = 0; i < 4; ++i) {
    const auto a = _mm256_extract_epi64(x, i);
    const auto b = _mm256_extract_epi64(y, i);
    if (a != b) return false;
  }
  return true;
}
}  // namespace

TEST_CASE("pxart::simd256::xrsr128p Copy") {
  pxart::simd256::xrsr128p rng1{std::random_device{}};
  auto rng2 = rng1;

  REQUIRE(equal(rng1.s0, rng2.s0));
  REQUIRE(equal(rng1.s1, rng2.s1));

  const int n = 1'000'000;
  for (int i = 0; i < n; ++i) {
    const auto rnd1 = rng1();
    const auto rnd2 = rng2();
    REQUIRE(equal(rnd1, rnd2));
  }
}

TEST_CASE("pxart::simd256::xrsr128p Vectorization") {
  pxart::simd256::xrsr128p rng1{std::random_device{}};
  pxart::xrsr128p rng2[4];
  for (int i = 0; i < 4; ++i) {
    rng2[i].s0 = reinterpret_cast<const uint64_t*>(&rng1.s0)[i];
    rng2[i].s1 = reinterpret_cast<const uint64_t*>(&rng1.s1)[i];
  }

  const int n = 10'000'000;
  for (int i = 0; i < n; i += 4) {
    const auto tmp = rng1();
    for (int j = 0; j < 4; ++j) {
      const auto rnd1 = reinterpret_cast<const uint64_t*>(&tmp)[j];
      const auto rnd2 = rng2[j]();
      REQUIRE(rnd1 == rnd2);
    }
  }
}

TEST_CASE("pxart::simd256::xrsr128p Jump Vectorization") {
  pxart::simd256::xrsr128p rng1{std::random_device{}};
  pxart::xrsr128p rng2[4];
  for (int i = 0; i < 4; ++i) {
    rng2[i].s0 = reinterpret_cast<const uint64_t*>(&rng1.s0)[i];
    rng2[i].s1 = reinterpret_cast<const uint64_t*>(&rng1.s1)[i];
  }

  rng1.jump();
  for (int it = 0; it < 4; ++it) {
    rng2[it].jump();
  }

  for (int i = 0; i < 4; ++i) {
    const auto tmp0 = reinterpret_cast<const uint64_t*>(&rng1.s0)[i];
    const auto tmp1 = reinterpret_cast<const uint64_t*>(&rng1.s1)[i];
    CHECK(rng2[i].s0 == tmp0);
    CHECK(rng2[i].s1 == tmp1);
  }

  const int n = 10'000'000;
  for (int i = 0; i < n; i += 4) {
    const auto tmp = rng1();
    for (int j = 0; j < 4; ++j) {
      const auto rnd1 = reinterpret_cast<const uint64_t*>(&tmp)[j];
      const auto rnd2 = rng2[j]();
      REQUIRE(rnd1 == rnd2);
    }
  }
}

TEST_CASE("pxart::simd256::xrsr128p Long Jump Vectorization") {
  pxart::simd256::xrsr128p rng1{std::random_device{}};
  pxart::xrsr128p rng2[4];
  for (int i = 0; i < 4; ++i) {
    rng2[i].s0 = reinterpret_cast<const uint64_t*>(&rng1.s0)[i];
    rng2[i].s1 = reinterpret_cast<const uint64_t*>(&rng1.s1)[i];
  }

  rng1.long_jump();
  for (int it = 0; it < 4; ++it) {
    rng2[it].long_jump();
  }

  for (int i = 0; i < 4; ++i) {
    const auto tmp0 = reinterpret_cast<const uint64_t*>(&rng1.s0)[i];
    const auto tmp1 = reinterpret_cast<const uint64_t*>(&rng1.s1)[i];
    CHECK(rng2[i].s0 == tmp0);
    CHECK(rng2[i].s1 == tmp1);
  }

  const int n = 10'000'000;
  for (int i = 0; i < n; i += 4) {
    const auto tmp = rng1();
    for (int j = 0; j < 4; ++j) {
      const auto rnd1 = reinterpret_cast<const uint64_t*>(&tmp)[j];
      const auto rnd2 = rng2[j]();
      REQUIRE(rnd1 == rnd2);
    }
  }
}