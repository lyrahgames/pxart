#include <doctest/doctest.h>
//
#include <random>
//
#include <pxart/distribution/uniform.hpp>
#include <pxart/generator/minstd_rand.hpp>

TEST_CASE("pxart::minstd_rand Default Initialization") {
  std::minstd_rand std_rng{};
  pxart::minstd_rand my_rng{};

  for (size_t i = 0; i < 10'000'000; ++i) {
    const auto std_rnd = std_rng();
    const auto my_rnd = my_rng();
    REQUIRE(my_rnd == std_rnd);
  }
}

TEST_CASE("pxart::minstd_rand Random Initialization") {
  for (size_t n = 0; n < 100'000; ++n) {
    const auto seed = std::random_device{}();
    std::minstd_rand std_rng{seed};
    pxart::minstd_rand my_rng{seed};

    for (size_t i = 0; i < 100; ++i) {
      const auto std_rnd = std_rng();
      const auto my_rnd = my_rng();
      REQUIRE(my_rnd == std_rnd);
    }
  }
}

TEST_CASE("pxart::minstd_rand Specialization of Default Uniform Distribution") {
  const auto seed = std::random_device{}();
  std::minstd_rand std_rng{seed};
  pxart::minstd_rand my_rng{seed};

  std::uniform_real_distribution<float> distribution{};

  for (size_t i = 0; i < 10'000'000; ++i) {
    const auto u1 = distribution(std_rng);
    const auto u2 = pxart::uniform<float>(my_rng);
    REQUIRE(u1 == u2);
  }
}