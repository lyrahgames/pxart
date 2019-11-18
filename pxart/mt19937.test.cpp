#include <doctest/doctest.h>

#include <pxart/mt19937.hpp>
#include <random>

TEST_CASE("pxart::mt19937 Default Initialization") {
  std::mt19937 rng{};
  pxart::mt19937 myrng{};

  const int n = 10'000'000;
  for (int i = 0; i < n; ++i) {
    const auto rnd = rng();
    const auto myrnd = myrng();
    REQUIRE(rnd == myrnd);
  }
}

TEST_CASE("pxart::mt19937 Random Initialization with Default Seeder") {
  const auto seed = std::random_device{}();
  std::mt19937 rng{seed};
  // pxart::mt19937::default_seeder seeder{seed};
  // pxart::mt19937 myrng{seeder};
  pxart::mt19937 myrng{pxart::mt19937::default_seeder{seed}};

  const int n = 10'000'000;
  for (int i = 0; i < n; ++i) {
    const auto rnd = rng();
    const auto myrnd = myrng();
    REQUIRE(rnd == myrnd);
  }
}

TEST_CASE("") {
  pxart::mt19937 rng{std::random_device{}};
  // auto rng2 = rng;
  pxart::mt19937 rng2 = rng;

  const int n = 10'000'000;
  for (int i = 0; i < n; ++i) {
    const auto rnd = rng();
    const auto rnd2 = rng2();
    REQUIRE(rnd == rnd2);
  }
}