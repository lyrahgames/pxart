#include <doctest/doctest.h>

#include <pxart/mt19937.hpp>
#include <pxart/uniform.hpp>
#include <random>

using namespace std;

namespace {

struct A : pxart::mt19937 {
  float uniform(float a, float b) noexcept { return -1.0f; }
};

}  // namespace

TEST_CASE("pxart::uniform Member Function Specialization") {
  A a{};
  pxart::mt19937 b{};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    REQUIRE(pxart::uniform(a, 0.0f, 1.0f) == -1.0f);
    const auto tmp = pxart::detail::uniform(a(), 0.0f, 1.0f);
    REQUIRE(pxart::uniform(b, 0.f, 1.f) == tmp);
    REQUIRE(tmp != -1.0f);
    // REQUIRE(pxart::uniform(b, 0., 1.) == pxart::uniform(a, 0., 1.));
    // REQUIRE(pxart::uniform(b, 0, 6) == pxart::uniform(a, 0, 6));
  }
}

TEST_CASE("pxart::detail::uniform<float>(uint32_t)") {
  random_device rd{};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform<float>(rd());
    REQUIRE(tmp <= 1.0f);
    REQUIRE(tmp >= 0.0f);
  }
}

TEST_CASE("pxart::detail::uniform<float>(uint32_t, float, float)") {
  random_device rd{};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform<float>(rd(), 1.0f, 1.55f);
    REQUIRE(tmp <= 1.55f);
    REQUIRE(tmp >= 1.0f);
  }
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform(rd(), 1.0f, 1.55f);
    REQUIRE(tmp <= 1.55f);
    REQUIRE(tmp >= 1.0f);
  }
}

TEST_CASE("pxart::detail::uniform<double>(uint32_t)") {
  random_device rd{};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform<double>(rd());
    REQUIRE(tmp <= 1.0);
    REQUIRE(tmp >= 0.0);
  }
}

TEST_CASE("pxart::detail::uniform<double>(uint32_t, double, double)") {
  random_device rd{};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform<double>(rd(), 1.0, 1.55);
    REQUIRE(tmp <= 1.55);
    REQUIRE(tmp >= 1.0);
  }
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform(rd(), 1.0, 1.55);
    REQUIRE(tmp <= 1.55);
    REQUIRE(tmp >= 1.0);
  }
}

TEST_CASE("pxart::detail::uniform<int>(uint32_t, int, int)") {
  random_device rd{};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform<int>(rd(), -23, 1023);
    REQUIRE(tmp < 1023);
    REQUIRE(tmp >= -23);
  }
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform(rd(), -42, -1);
    REQUIRE(tmp < -1);
    REQUIRE(tmp >= -42);
  }
}

TEST_CASE("pxart::detail::uniform<long>(uint64_t, long, long)") {
  pxart::mt19937 rng{random_device{}};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform<long>(
        static_cast<uint64_t>(rng()) << 32 | rng(), -23l, 1023l);
    REQUIRE(tmp < 1023);
    REQUIRE(tmp >= -23);
  }
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform(
        static_cast<uint64_t>(rng()) << 32 | rng(), -42l, -1l);
    REQUIRE(tmp < -1);
    REQUIRE(tmp >= -42);
  }
}

TEST_CASE("pxart::detail::uniform<float>(uint64_t)") {
  random_device rd{};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform<float>(
        (static_cast<uint64_t>(rd()) << 32) | rd());
    REQUIRE(tmp <= 1.0f);
    REQUIRE(tmp >= 0.0f);
  }
}

TEST_CASE("pxart::detail::uniform<float>(uint64_t, float, float)") {
  random_device rd{};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform<float>(
        (static_cast<uint64_t>(rd()) << 32) | rd(), 1.0f, 1.55f);
    REQUIRE(tmp <= 1.55f);
    REQUIRE(tmp >= 1.0f);
  }
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform(
        (static_cast<uint64_t>(rd()) << 32) | rd(), 1.0f, 1.55f);
    REQUIRE(tmp <= 1.55f);
    REQUIRE(tmp >= 1.0f);
  }
}

TEST_CASE("pxart::detail::uniform<double>(uint64_t)") {
  random_device rd{};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform<double>(
        (static_cast<uint64_t>(rd()) << 32) | rd());
    REQUIRE(tmp <= 1.0);
    REQUIRE(tmp >= 0.0);
  }
}

TEST_CASE("pxart::detail::uniform<double>(uint64_t, double, double)") {
  random_device rd{};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform<double>(
        (static_cast<uint64_t>(rd()) << 32) | rd(), 1.0, 1.55);
    REQUIRE(tmp <= 1.55);
    REQUIRE(tmp >= 1.0);
  }
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform(
        (static_cast<uint64_t>(rd()) << 32) | rd(), 1.0, 1.55);
    REQUIRE(tmp <= 1.55);
    REQUIRE(tmp >= 1.0);
  }
}

TEST_CASE("pxart::uniform<float>(RNG, float, float)") {
  random_device rd{};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::uniform<float>(rd, 1.0f, 1.55f);
    REQUIRE(tmp <= 1.55f);
    REQUIRE(tmp >= 1.0f);
  }
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::uniform(rd, 1.0f, 1.55f);
    REQUIRE(tmp <= 1.55f);
    REQUIRE(tmp >= 1.0f);
  }
}