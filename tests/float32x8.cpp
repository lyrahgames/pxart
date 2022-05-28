#include <array>
#include <cstdint>
#include <type_traits>
#include <vector>
//
#include <doctest/doctest.h>
//
#include <pxart/float32x8.hpp>
#include <pxart/utility.hpp>

using namespace std;

#ifdef __AVX2__

TEST_CASE("pxart::simd256::float32x8 default constructor") {
  pxart::simd256::float32x8 v{};
  const auto sv = pxart::pun_cast<array<float, 8>>(v);
  for (const auto x : sv) CHECK(x == 0.0f);
}

TEST_CASE("pxart::simd256::float32x8 one element constructor") {
  pxart::simd256::float32x8 v{1.23f};
  const auto sv = pxart::pun_cast<array<float, 8>>(v);
  for (const auto x : sv) CHECK(x == 1.23f);
}

TEST_CASE("pxart::simd256::float32x8 element constructor") {
  pxart::simd256::float32x8 v{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f};
  const auto sv = pxart::pun_cast<array<float, 8>>(v);
  CHECK(sv[0] == 1.0f);
  CHECK(sv[1] == 2.0f);
  CHECK(sv[2] == 3.0f);
  CHECK(sv[3] == 4.0f);
  CHECK(sv[4] == 5.0f);
  CHECK(sv[5] == 6.0f);
  CHECK(sv[6] == 7.0f);
  CHECK(sv[7] == 8.0f);
}

TEST_CASE("pxart::simd256::float32x8 reverse element constructor") {
  using pxart::simd256::float32x8;
  float32x8 v{
      1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, float32x8::reverse};
  const auto sv = pxart::pun_cast<array<float, 8>>(v);
  CHECK(sv[0] == 8.0f);
  CHECK(sv[1] == 7.0f);
  CHECK(sv[2] == 6.0f);
  CHECK(sv[3] == 5.0f);
  CHECK(sv[4] == 4.0f);
  CHECK(sv[5] == 3.0f);
  CHECK(sv[6] == 2.0f);
  CHECK(sv[7] == 1.0f);
}

TEST_CASE("pxart::simd256::float32x8 implicit casts and assignments") {
  using pxart::simd256::float32x8;

  float32x8 v{1, 2, 3, 4, 5, 6, 7, 8};
  __m256 vm = v;
  const auto svm = pxart::pun_cast<array<float, 8>>(vm);
  CHECK(svm[0] == 1.0f);
  CHECK(svm[1] == 2.0f);
  CHECK(svm[2] == 3.0f);
  CHECK(svm[3] == 4.0f);
  CHECK(svm[4] == 5.0f);
  CHECK(svm[5] == 6.0f);
  CHECK(svm[6] == 7.0f);
  CHECK(svm[7] == 8.0f);

  __m256 mm = _mm256_set1_ps(1.0f);
  __m256 vm2 = _mm256_add_ps(mm, v);
  const auto svm2 = pxart::pun_cast<array<float, 8>>(vm2);
  CHECK(svm2[0] == 2.0f);
  CHECK(svm2[1] == 3.0f);
  CHECK(svm2[2] == 4.0f);
  CHECK(svm2[3] == 5.0f);
  CHECK(svm2[4] == 6.0f);
  CHECK(svm2[5] == 7.0f);
  CHECK(svm2[6] == 8.0f);
  CHECK(svm2[7] == 9.0f);

  // float32x8 w{_mm256_set1_ps(2.0f)};
  float32x8 w = _mm256_set1_ps(2.0f);
  auto sw = pxart::pun_cast<array<float, 8>>(w);
  for (const auto x : sw) CHECK(x == 2.0f);
  w = _mm256_set1_ps(3.0f);
  sw = pxart::pun_cast<array<float, 8>>(w);
  for (const auto x : sw) CHECK(x == 3.0f);
}

TEST_CASE("pxart::simd256::float32x8 unaligned load constructor") {
  using pxart::simd256::float32x8;
  vector<float> data = {1, 2, 3, 4, 5, 6, 7, 8};
  float32x8 v{data.data()};
  // float32x8 v{data.data(), float32x8::unaligned};
  const auto sv = pxart::pun_cast<array<float, 8>>(v);
  CHECK(sv[0] == 1.0f);
  CHECK(sv[1] == 2.0f);
  CHECK(sv[2] == 3.0f);
  CHECK(sv[3] == 4.0f);
  CHECK(sv[4] == 5.0f);
  CHECK(sv[5] == 6.0f);
  CHECK(sv[6] == 7.0f);
  CHECK(sv[7] == 8.0f);
}

TEST_CASE("pxart::simd256::float32x8 aligned load constructor") {
  using pxart::simd256::float32x8;

  // Construct aligned storage on heap.
  vector<float> data(16);
  // MESSAGE(reinterpret_cast<uint64_t>(data.data())
  //         << '\t' << reinterpret_cast<uint64_t>(data.data()) % 32);
  const uint64_t start =
      (32 - (reinterpret_cast<uint64_t>(data.data()) % 32)) >> 2;
  // MESSAGE(start << '\t'
  //               << reinterpret_cast<uint64_t>(data.data() + start) % 32);
  for (int i = 0; i < 8; ++i) data[start + i] = i + 1;

  float32x8 v{data.data() + start, float32x8::aligned};
  const auto sv = pxart::pun_cast<array<float, 8>>(v);
  CHECK(sv[0] == 1.0f);
  CHECK(sv[1] == 2.0f);
  CHECK(sv[2] == 3.0f);
  CHECK(sv[3] == 4.0f);
  CHECK(sv[4] == 5.0f);
  CHECK(sv[5] == 6.0f);
  CHECK(sv[6] == 7.0f);
  CHECK(sv[7] == 8.0f);
}

TEST_CASE("pxart::simd256::float32x8 operations") {
  using pxart::simd256::float32x8;
  float32x8 x{1, 2, 3, 4, 5, 6, 7, 8};
  float32x8 y{10, 20, 30, 40, 50, 60, 70, 80};

  SUBCASE("addition") {
    auto z = x + y;
    static_assert(is_same_v<float32x8, decltype(z)>);
    auto sz = pxart::pun_cast<array<float, 8>>(z);
    CHECK(sz[0] == 11.0f);
    CHECK(sz[1] == 22.0f);
    CHECK(sz[2] == 33.0f);
    CHECK(sz[3] == 44.0f);
    CHECK(sz[4] == 55.0f);
    CHECK(sz[5] == 66.0f);
    CHECK(sz[6] == 77.0f);
    CHECK(sz[7] == 88.0f);
  }

  SUBCASE("positive") {
    auto z = +x;
    static_assert(is_same_v<float32x8, decltype(z)>);
    auto sz = pxart::pun_cast<array<float, 8>>(z);
    CHECK(sz[0] == 1.0f);
    CHECK(sz[1] == 2.0f);
    CHECK(sz[2] == 3.0f);
    CHECK(sz[3] == 4.0f);
    CHECK(sz[4] == 5.0f);
    CHECK(sz[5] == 6.0f);
    CHECK(sz[6] == 7.0f);
    CHECK(sz[7] == 8.0f);
  }

  SUBCASE("subtraction") {
    auto z = x - y;
    static_assert(is_same_v<float32x8, decltype(z)>);
    auto sz = pxart::pun_cast<array<float, 8>>(z);
    CHECK(sz[0] == -9.0f);
    CHECK(sz[1] == -18.0f);
    CHECK(sz[2] == -27.0f);
    CHECK(sz[3] == -36.0f);
    CHECK(sz[4] == -45.0f);
    CHECK(sz[5] == -54.0f);
    CHECK(sz[6] == -63.0f);
    CHECK(sz[7] == -72.0f);
  }

  SUBCASE("negation") {
    auto z = -x;
    static_assert(is_same_v<float32x8, decltype(z)>);
    auto sz = pxart::pun_cast<array<float, 8>>(z);
    CHECK(sz[0] == -1.0f);
    CHECK(sz[1] == -2.0f);
    CHECK(sz[2] == -3.0f);
    CHECK(sz[3] == -4.0f);
    CHECK(sz[4] == -5.0f);
    CHECK(sz[5] == -6.0f);
    CHECK(sz[6] == -7.0f);
    CHECK(sz[7] == -8.0f);
  }

  SUBCASE("multiplication") {
    auto z = x * y;
    auto sz = pxart::pun_cast<array<float, 8>>(z);
    CHECK(sz[0] == 10.0f);
    CHECK(sz[1] == 40.0f);
    CHECK(sz[2] == 90.0f);
    CHECK(sz[3] == 160.0f);
    CHECK(sz[4] == 250.0f);
    CHECK(sz[5] == 360.0f);
    CHECK(sz[6] == 490.0f);
    CHECK(sz[7] == 640.0f);
  }

  SUBCASE("division") {
    auto z = x / y;
    static_assert(is_same_v<float32x8, decltype(z)>);
    auto sz = pxart::pun_cast<array<float, 8>>(z);
    for (const auto x : sz) CHECK(x == 0.1f);
  }

  SUBCASE("addsub") {
    auto z = addsub(x, y);
    static_assert(is_same_v<float32x8, decltype(z)>);
    auto sz = pxart::pun_cast<array<float, 8>>(z);
    CHECK(sz[0] == -9.0f);
    CHECK(sz[1] == 22.0f);
    CHECK(sz[2] == -27.0f);
    CHECK(sz[3] == 44.0f);
    CHECK(sz[4] == -45.0f);
    CHECK(sz[5] == 66.0f);
    CHECK(sz[6] == -63.0f);
    CHECK(sz[7] == 88.0f);
  }

  SUBCASE("unpack_high_halfs") {
    auto z = unpack_high_halfs(x, y);
    auto sz = pxart::pun_cast<array<float, 8>>(z);
    CHECK(sz[0] == 3.0f);
    CHECK(sz[1] == 30.0f);
    CHECK(sz[2] == 4.0f);
    CHECK(sz[3] == 40.0f);
    CHECK(sz[4] == 7.0f);
    CHECK(sz[5] == 70.0f);
    CHECK(sz[6] == 8.0f);
    CHECK(sz[7] == 80.0f);
  }

  SUBCASE("unpack_low_halfs") {
    auto z = unpack_low_halfs(x, y);
    auto sz = pxart::pun_cast<array<float, 8>>(z);
    CHECK(sz[0] == 1.0f);
    CHECK(sz[1] == 10.0f);
    CHECK(sz[2] == 2.0f);
    CHECK(sz[3] == 20.0f);
    CHECK(sz[4] == 5.0f);
    CHECK(sz[5] == 50.0f);
    CHECK(sz[6] == 6.0f);
    CHECK(sz[7] == 60.0f);
  }

  SUBCASE("duplicate_odd") {
    auto z = duplicate_odd(x);
    auto sz = pxart::pun_cast<array<float, 8>>(z);
    CHECK(sz[0] == 2.0f);
    CHECK(sz[1] == 2.0f);
    CHECK(sz[2] == 4.0f);
    CHECK(sz[3] == 4.0f);
    CHECK(sz[4] == 6.0f);
    CHECK(sz[5] == 6.0f);
    CHECK(sz[6] == 8.0f);
    CHECK(sz[7] == 8.0f);
  }

  SUBCASE("duplicate_even") {
    auto z = duplicate_even(x);
    auto sz = pxart::pun_cast<array<float, 8>>(z);
    CHECK(sz[0] == 1.0f);
    CHECK(sz[1] == 1.0f);
    CHECK(sz[2] == 3.0f);
    CHECK(sz[3] == 3.0f);
    CHECK(sz[4] == 5.0f);
    CHECK(sz[5] == 5.0f);
    CHECK(sz[6] == 7.0f);
    CHECK(sz[7] == 7.0f);
  }

  SUBCASE("") {
    auto z = inlane_permutation<0, 1, 1, 2>(x);
    auto sz = pxart::pun_cast<array<float, 8>>(z);
    for (size_t i = 0; i < 8; ++i) MESSAGE(sz[i]);
  }
  SUBCASE("") {
    auto z = lane_permutation<0, 2>(x, y);
    z[0] = 123;
    auto sz = pxart::pun_cast<array<float, 8>>(z);
    for (size_t i = 0; i < 8; ++i) MESSAGE(z[i]);
  }
}

#endif  // __AVX2__