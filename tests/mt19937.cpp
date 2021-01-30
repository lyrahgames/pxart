#include <doctest/doctest.h>
//
#include <random>
//
#include <pxart/distribution/binomial.hpp>
#include <pxart/distribution/exponential.hpp>
#include <pxart/distribution/normal.hpp>
#include <pxart/distribution/uniform.hpp>
#include <pxart/generator/mt19937.hpp>

static_assert(pxart::generic::random_bit_generator<pxart::mt19937>);
static_assert(
    pxart::generic::equal<pxart::meta::result<pxart::mt19937>, uint32_t>);

static_assert(
    pxart::generic::random_bit_generator<pxart::mt19937::default_seeder>);
static_assert(pxart::generic::equal<
              pxart::meta::result<pxart::mt19937::default_seeder>, uint32_t>);

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
  std::random_device seeder{};

  for (int i = 0; i < 1000; ++i) {
    const auto seed = seeder();
    std::mt19937 rng{seed};
    pxart::mt19937 myrng{pxart::mt19937::default_seeder{seed}};

    for (int j = 0; j < 10'000; ++j) {
      const auto rnd = rng();
      const auto myrnd = myrng();
      REQUIRE(rnd == myrnd);
    }
  }
}
