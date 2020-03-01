#include <doctest/doctest.h>

#include <pxart/uniform_real_distribution.hpp>
#include <random>

TEST_CASE("uniform_real_distribution") {
  const auto seed = std::random_device{}();
  std::mt19937 rng1{seed};
  auto rng2 = rng1;
  std::uniform_real_distribution<double> stdd{0, 1};
  pxart::uniform_real_distribution<double> rngd{0, 1};

  for (int i = 0; i < 20; ++i) {
    const auto first = stdd(rng1);
    const auto second = rngd(rng2);
    MESSAGE("\n" << first << "\t" << second);
  }
}