#include <doctest/doctest.h>

#include <pxart/xoroshiro128plus.hpp>
#include <random>

TEST_CASE("pxart::xrsr128p Copy") {
  pxart::xrsr128p rng1{std::random_device{}};
  auto rng2 = rng1;

  REQUIRE(rng1.s0 == rng2.s0);
  REQUIRE(rng1.s1 == rng2.s1);

  const int n = 10'000'000;
  for (int i = 0; i < n; ++i) {
    const auto rnd1 = rng1();
    const auto rnd2 = rng2();
    REQUIRE(rnd1 == rnd2);
  }
}

TEST_CASE("pxart::xrsr128p Jump" * doctest::skip(true)) {
  pxart::xrsr128p rng1{std::random_device{}};
  pxart::xrsr128p rng2 = rng1;

  rng2.long_jump();
  for (uint64_t i = 0; i < (1ull << 32); ++i) rng1.jump();

  REQUIRE(rng1.s0 == rng2.s0);
  REQUIRE(rng1.s1 == rng2.s1);

  const int n = 1'000'000;
  for (int i = 0; i < n; ++i) {
    const auto rnd1 = rng1();
    const auto rnd2 = rng2();
    REQUIRE(rnd1 == rnd2);
  }
}