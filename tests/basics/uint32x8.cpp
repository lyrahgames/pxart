#include <doctest/doctest.h>

#include <array>
#include <cstdint>
#include <pxart/utility/pun_cast.hpp>
#include <pxart/utility/uint32x8.hpp>
#include <type_traits>
#include <vector>

using namespace std;

TEST_CASE("pxart::simd256::uint32x8 default constructor") {
  using pxart::simd256::uint32x8;
  uint32x8 v{};
  const auto sv = pxart::pun_cast<array<uint32_t, 8>>(v);
  for (const auto x : sv) CHECK(x == 0);
}

TEST_CASE("pxart::simd256::uint32x8 one element constructor") {
  pxart::simd256::uint32x8 v{123};
  const auto sv = pxart::pun_cast<array<uint32_t, 8>>(v);
  for (const auto x : sv) CHECK(x == 123);
}

TEST_CASE("pxart::simd256::uint32x8 element constructor") {
  pxart::simd256::uint32x8 v{1, 2, 3, 4, 5, 6, 7, 8};
  const auto sv = pxart::pun_cast<array<uint32_t, 8>>(v);
  CHECK(sv[0] == 1);
  CHECK(sv[1] == 2);
  CHECK(sv[2] == 3);
  CHECK(sv[3] == 4);
  CHECK(sv[4] == 5);
  CHECK(sv[5] == 6);
  CHECK(sv[6] == 7);
  CHECK(sv[7] == 8);
}

TEST_CASE("pxart::simd256::uint32x8 reverse element constructor") {
  using pxart::simd256::uint32x8;
  uint32x8 v{1, 2, 3, 4, 5, 6, 7, 8, uint32x8::reverse};
  const auto sv = pxart::pun_cast<array<uint32_t, 8>>(v);
  CHECK(sv[0] == 8);
  CHECK(sv[1] == 7);
  CHECK(sv[2] == 6);
  CHECK(sv[3] == 5);
  CHECK(sv[4] == 4);
  CHECK(sv[5] == 3);
  CHECK(sv[6] == 2);
  CHECK(sv[7] == 1);
}

TEST_CASE("pxart::simd256::uint32x8 implicit casts and assignments") {
  using pxart::simd256::uint32x8;

  uint32x8 v{1, 2, 3, 4, 5, 6, 7, 8};
  __m256i vm = v;
  const auto svm = pxart::pun_cast<array<uint32_t, 8>>(vm);
  CHECK(svm[0] == 1);
  CHECK(svm[1] == 2);
  CHECK(svm[2] == 3);
  CHECK(svm[3] == 4);
  CHECK(svm[4] == 5);
  CHECK(svm[5] == 6);
  CHECK(svm[6] == 7);
  CHECK(svm[7] == 8);

  __m256i mm = _mm256_set1_epi32(1);
  __m256i vm2 = _mm256_add_epi32(mm, v);
  const auto svm2 = pxart::pun_cast<array<uint32_t, 8>>(vm2);
  CHECK(svm2[0] == 2);
  CHECK(svm2[1] == 3);
  CHECK(svm2[2] == 4);
  CHECK(svm2[3] == 5);
  CHECK(svm2[4] == 6);
  CHECK(svm2[5] == 7);
  CHECK(svm2[6] == 8);
  CHECK(svm2[7] == 9);

  // uint32x8 w{_mm256_set1_epi32(2)};
  uint32x8 w = _mm256_set1_epi32(2);
  auto sw = pxart::pun_cast<array<uint32_t, 8>>(w);
  for (const auto x : sw) CHECK(x == 2);
  w = _mm256_set1_epi32(3);
  sw = pxart::pun_cast<array<uint32_t, 8>>(w);
  for (const auto x : sw) CHECK(x == 3);
}

TEST_CASE("pxart::simd256::uint32x8 unaligned load constructor") {
  using pxart::simd256::uint32x8;
  vector<uint32_t> data = {1, 2, 3, 4, 5, 6, 7, 8};
  uint32x8 v{data.data()};
  // uint32x8 v{data.data(), uint32x8::unaligned};
  const auto sv = pxart::pun_cast<array<uint32_t, 8>>(v);
  CHECK(sv[0] == 1);
  CHECK(sv[1] == 2);
  CHECK(sv[2] == 3);
  CHECK(sv[3] == 4);
  CHECK(sv[4] == 5);
  CHECK(sv[5] == 6);
  CHECK(sv[6] == 7);
  CHECK(sv[7] == 8);
}

TEST_CASE("pxart::simd256::uint32x8 aligned load constructor") {
  using pxart::simd256::uint32x8;

  // Construct aligned storage on heap.
  vector<uint32_t> data(16);
  // MESSAGE(reinterpret_cast<uint64_t>(data.data())
  //         << '\t' << reinterpret_cast<uint64_t>(data.data()) % 32);
  const uint64_t start =
      (32 - (reinterpret_cast<uint64_t>(data.data()) % 32)) >> 2;
  // MESSAGE(start << '\t'
  //               << reinterpret_cast<uint64_t>(data.data() + start) % 32);
  for (int i = 0; i < 8; ++i) data[start + i] = i + 1;

  uint32x8 v{data.data() + start, uint32x8::aligned};
  const auto sv = pxart::pun_cast<array<uint32_t, 8>>(v);
  CHECK(sv[0] == 1);
  CHECK(sv[1] == 2);
  CHECK(sv[2] == 3);
  CHECK(sv[3] == 4);
  CHECK(sv[4] == 5);
  CHECK(sv[5] == 6);
  CHECK(sv[6] == 7);
  CHECK(sv[7] == 8);
}

TEST_CASE("pxart::simd256::uint32x8 operations") {
  using pxart::simd256::uint32x8;
  uint32x8 x{1, 2, 3, 4, 5, 6, 7, 8};
  uint32x8 y{10, 20, 30, 40, 50, 60, 70, 80};

  SUBCASE("addition") {
    auto z = x + y;
    auto sz = pxart::pun_cast<array<uint32_t, 8>>(z);
    CHECK(sz[0] == 11);
    CHECK(sz[1] == 22);
    CHECK(sz[2] == 33);
    CHECK(sz[3] == 44);
    CHECK(sz[4] == 55);
    CHECK(sz[5] == 66);
    CHECK(sz[6] == 77);
    CHECK(sz[7] == 88);
  }

  SUBCASE("positive") {
    auto z = +x;
    auto sz = pxart::pun_cast<array<uint32_t, 8>>(z);
    CHECK(sz[0] == 1);
    CHECK(sz[1] == 2);
    CHECK(sz[2] == 3);
    CHECK(sz[3] == 4);
    CHECK(sz[4] == 5);
    CHECK(sz[5] == 6);
    CHECK(sz[6] == 7);
    CHECK(sz[7] == 8);
  }

  SUBCASE("subtraction") {
    auto z = x - y;
    auto sz = pxart::pun_cast<array<uint32_t, 8>>(z);
    CHECK(sz[0] == -9);
    CHECK(sz[1] == -18);
    CHECK(sz[2] == -27);
    CHECK(sz[3] == -36);
    CHECK(sz[4] == -45);
    CHECK(sz[5] == -54);
    CHECK(sz[6] == -63);
    CHECK(sz[7] == -72);
  }

  SUBCASE("negation") {
    auto z = -x;
    auto sz = pxart::pun_cast<array<uint32_t, 8>>(z);
    CHECK(sz[0] == -1);
    CHECK(sz[1] == -2);
    CHECK(sz[2] == -3);
    CHECK(sz[3] == -4);
    CHECK(sz[4] == -5);
    CHECK(sz[5] == -6);
    CHECK(sz[6] == -7);
    CHECK(sz[7] == -8);
  }

  SUBCASE("horizontal addition") {
    auto z = hadd(x, y);
    auto sz = pxart::pun_cast<array<uint32_t, 8>>(z);
    CHECK(sz[0] == 3);
    CHECK(sz[1] == 7);
    CHECK(sz[2] == 30);
    CHECK(sz[3] == 70);
    CHECK(sz[4] == 11);
    CHECK(sz[5] == 15);
    CHECK(sz[6] == 110);
    CHECK(sz[7] == 150);
  }

  SUBCASE("horizontal subtraction") {
    auto z = hsub(x, y);
    auto sz = pxart::pun_cast<array<uint32_t, 8>>(z);
    CHECK(sz[0] == -1);
    CHECK(sz[1] == -1);
    CHECK(sz[2] == -10);
    CHECK(sz[3] == -10);
    CHECK(sz[4] == -1);
    CHECK(sz[5] == -1);
    CHECK(sz[6] == -10);
    CHECK(sz[7] == -10);
  }

  SUBCASE("maximum") {
    auto z = max(x, 5);
    auto sz = pxart::pun_cast<array<uint32_t, 8>>(z);
    CHECK(sz[0] == 5);
    CHECK(sz[1] == 5);
    CHECK(sz[2] == 5);
    CHECK(sz[3] == 5);
    CHECK(sz[4] == 5);
    CHECK(sz[5] == 6);
    CHECK(sz[6] == 7);
    CHECK(sz[7] == 8);

    z = max(x, {2, 1, 4, 3, 6, 5, 8, 7});
    sz = pxart::pun_cast<array<uint32_t, 8>>(z);
    CHECK(sz[0] == 2);
    CHECK(sz[1] == 2);
    CHECK(sz[2] == 4);
    CHECK(sz[3] == 4);
    CHECK(sz[4] == 6);
    CHECK(sz[5] == 6);
    CHECK(sz[6] == 8);
    CHECK(sz[7] == 8);
  }

  SUBCASE("minimum") {
    auto z = min(x, 5);
    auto sz = pxart::pun_cast<array<uint32_t, 8>>(z);
    CHECK(sz[0] == 1);
    CHECK(sz[1] == 2);
    CHECK(sz[2] == 3);
    CHECK(sz[3] == 4);
    CHECK(sz[4] == 5);
    CHECK(sz[5] == 5);
    CHECK(sz[6] == 5);
    CHECK(sz[7] == 5);

    z = min(x, {2, 1, 4, 3, 6, 5, 8, 7});
    sz = pxart::pun_cast<array<uint32_t, 8>>(z);
    CHECK(sz[0] == 1);
    CHECK(sz[1] == 1);
    CHECK(sz[2] == 3);
    CHECK(sz[3] == 3);
    CHECK(sz[4] == 5);
    CHECK(sz[5] == 5);
    CHECK(sz[6] == 7);
    CHECK(sz[7] == 7);
  }

  SUBCASE("multiplication") {
    auto z = x * y;
    auto sz = pxart::pun_cast<array<uint32_t, 8>>(z);
    CHECK(sz[0] == 10);
    CHECK(sz[1] == 40);
    CHECK(sz[2] == 90);
    CHECK(sz[3] == 160);
    CHECK(sz[4] == 250);
    CHECK(sz[5] == 360);
    CHECK(sz[6] == 490);
    CHECK(sz[7] == 640);
  }

  SUBCASE("shift left") {
    auto z = x << 2;
    auto sz = pxart::pun_cast<array<uint32_t, 8>>(z);
    CHECK(sz[0] == 4);
    CHECK(sz[1] == 8);
    CHECK(sz[2] == 12);
    CHECK(sz[3] == 16);
    CHECK(sz[4] == 20);
    CHECK(sz[5] == 24);
    CHECK(sz[6] == 28);
    CHECK(sz[7] == 32);

    z = x << uint32x8{3, 3, 2, 2, 1, 1, 0, 0};
    sz = pxart::pun_cast<array<uint32_t, 8>>(z);
    CHECK(sz[0] == 8);
    CHECK(sz[1] == 16);
    CHECK(sz[2] == 12);
    CHECK(sz[3] == 16);
    CHECK(sz[4] == 10);
    CHECK(sz[5] == 12);
    CHECK(sz[6] == 7);
    CHECK(sz[7] == 8);
  }

  SUBCASE("shift right") {
    auto z = y >> 2;
    auto sz = pxart::pun_cast<array<uint32_t, 8>>(z);
    CHECK(sz[0] == 2);
    CHECK(sz[1] == 5);
    CHECK(sz[2] == 7);
    CHECK(sz[3] == 10);
    CHECK(sz[4] == 12);
    CHECK(sz[5] == 15);
    CHECK(sz[6] == 17);
    CHECK(sz[7] == 20);

    z = y >> uint32x8{3, 3, 2, 2, 1, 1, 0, 0, uint32x8::reverse};
    sz = pxart::pun_cast<array<uint32_t, 8>>(z);
    CHECK(sz[0] == 10);
    CHECK(sz[1] == 20);
    CHECK(sz[2] == 15);
    CHECK(sz[3] == 20);
    CHECK(sz[4] == 12);
    CHECK(sz[5] == 15);
    CHECK(sz[6] == 8);
    CHECK(sz[7] == 10);
  }

  // SUBCASE("unpack_high_halfs") {
  //   auto z = unpack_high_halfs(x, y);
  //   auto sz = pxart::pun_cast<array<uint32_t, 8>>(z);
  //   CHECK(sz[0] == 3);
  //   CHECK(sz[1] == 30);
  //   CHECK(sz[2] == 4);
  //   CHECK(sz[3] == 40);
  //   CHECK(sz[4] == 7);
  //   CHECK(sz[5] == 70);
  //   CHECK(sz[6] == 8);
  //   CHECK(sz[7] == 80);
  // }

  // SUBCASE("unpack_low_halfs") {
  //   auto z = unpack_low_halfs(x, y);
  //   auto sz = pxart::pun_cast<array<uint32_t, 8>>(z);
  //   CHECK(sz[0] == 1);
  //   CHECK(sz[1] == 10);
  //   CHECK(sz[2] == 2);
  //   CHECK(sz[3] == 20);
  //   CHECK(sz[4] == 5);
  //   CHECK(sz[5] == 50);
  //   CHECK(sz[6] == 6);
  //   CHECK(sz[7] == 60);
  // }
}