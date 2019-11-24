#include <doctest/doctest.h>

#include <pxart/mt19937.hpp>
#include <pxart/uniform.hpp>
#include <random>

using namespace std;

TEST_CASE("pxart::detail::uniform<float>(uint32_t)") {
  random_device rd{};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform<float>(rd());
    CHECK(tmp <= 1.0f);
    CHECK(tmp >= 0.0f);
  }
}

TEST_CASE("pxart::detail::uniform<float>(uint32_t, float, float)") {
  random_device rd{};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform<float>(rd(), 1.0, 1.55);
    CHECK(tmp <= 1.55f);
    CHECK(tmp >= 1.0f);
  }
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform(rd(), 1.0f, 1.55f);
    CHECK(tmp <= 1.55f);
    CHECK(tmp >= 1.0f);
  }
}

TEST_CASE("pxart::detail::uniform<double>(uint32_t)") {
  random_device rd{};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform<double>(rd());
    CHECK(tmp <= 1.0);
    CHECK(tmp >= 0.0);
  }
}

TEST_CASE("pxart::detail::uniform<double>(uint32_t, double, double)") {
  random_device rd{};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform<double>(rd(), 1.0, 1.55);
    CHECK(tmp <= 1.55);
    CHECK(tmp >= 1.0);
  }
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform(rd(), 1.0, 1.55);
    CHECK(tmp <= 1.55);
    CHECK(tmp >= 1.0);
  }
}

TEST_CASE("pxart::detail::uniform<int>(uint32_t, int, int)") {
  random_device rd{};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform<int>(rd(), -23, 1023);
    CHECK(tmp < 1023);
    CHECK(tmp >= -23);
  }
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform(rd(), -42, -1);
    CHECK(tmp < -1);
    CHECK(tmp >= -42);
  }
}

TEST_CASE("pxart::detail::uniform<long>(uint64_t, long, long)") {
  pxart::mt19937 rng{random_device{}};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform<long>(
        static_cast<uint64_t>(rng()) << 32 | rng(), -23l, 1023l);
    CHECK(tmp < 1023);
    CHECK(tmp >= -23);
  }
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform(
        static_cast<uint64_t>(rng()) << 32 | rng(), -42l, -1l);
    CHECK(tmp < -1);
    CHECK(tmp >= -42);
  }
}

TEST_CASE("pxart::detail::uniform<float>(uint64_t)") {
  random_device rd{};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform<float>(
        (static_cast<uint64_t>(rd()) << 32) | rd());
    CHECK(tmp <= 1.0f);
    CHECK(tmp >= 0.0f);
  }
}

TEST_CASE("pxart::detail::uniform<float>(uint64_t, float, float)") {
  random_device rd{};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform<float>(
        (static_cast<uint64_t>(rd()) << 32) | rd(), 1.0, 1.55);
    CHECK(tmp <= 1.55f);
    CHECK(tmp >= 1.0f);
  }
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform(
        (static_cast<uint64_t>(rd()) << 32) | rd(), 1.0f, 1.55f);
    CHECK(tmp <= 1.55f);
    CHECK(tmp >= 1.0f);
  }
}

TEST_CASE("pxart::detail::uniform<double>(uint64_t)") {
  random_device rd{};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform<double>(
        (static_cast<uint64_t>(rd()) << 32) | rd());
    CHECK(tmp <= 1.0);
    CHECK(tmp >= 0.0);
  }
}

TEST_CASE("pxart::detail::uniform<double>(uint64_t, double, double)") {
  random_device rd{};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform<double>(
        (static_cast<uint64_t>(rd()) << 32) | rd(), 1.0, 1.55);
    CHECK(tmp <= 1.55);
    CHECK(tmp >= 1.0);
  }
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::detail::uniform(
        (static_cast<uint64_t>(rd()) << 32) | rd(), 1.0, 1.55);
    CHECK(tmp <= 1.55);
    CHECK(tmp >= 1.0);
  }
}

TEST_CASE("pxart::uniform<float>(RNG, float, float)") {
  random_device rd{};
  const int n = 100'000;
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::uniform<float>(rd, 1.0, 1.55);
    CHECK(tmp <= 1.55f);
    CHECK(tmp >= 1.0f);
  }
  for (int i = 0; i < n; ++i) {
    const auto tmp = pxart::uniform(rd, 1.0f, 1.55f);
    CHECK(tmp <= 1.55f);
    CHECK(tmp >= 1.0f);
  }
}