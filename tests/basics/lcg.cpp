#include <doctest/doctest.h>
//
#include <iomanip>
#include <iostream>
#include <random>
//
#include <pxart/lcg.hpp>

using namespace std;

TEST_CASE("pxart::minstd_rand default initialization") {
  std::minstd_rand rng{};
  pxart::minstd_rand myrng{};

  const int n = 10'000'000;
  for (int i = 0; i < n; ++i) {
    const auto rnd = rng();
    const auto myrnd = myrng();
    REQUIRE(rnd == myrnd);
  }
}

TEST_CASE("pxart::minstd_rand random initialization") {
  const auto seed = std::random_device{}();
  std::minstd_rand rng{seed};
  pxart::minstd_rand myrng{seed};

  const int n = 10'000'000;
  for (int i = 0; i < n; ++i) {
    const auto rnd = rng();
    const auto myrnd = myrng();
    REQUIRE(rnd == myrnd);
  }
}
