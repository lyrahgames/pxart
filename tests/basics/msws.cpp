#include <doctest/doctest.h>

#include <pxart/msws.hpp>
#include <random>

TEST_CASE("pxart::msws Copy") {
  pxart::msws rng1{std::random_device{}};
  auto rng2 = rng1;

  const int n = 10'000'000;
  for (int i = 0; i < n; ++i) {
    const auto rnd1 = rng1();
    const auto rnd2 = rng2();
    REQUIRE(rnd1 == rnd2);
  }
}