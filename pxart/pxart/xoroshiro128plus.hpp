#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <pxart/algorithm.hpp>

namespace pxart {

struct xoroshiro128plus {
  using uint_type = uint64_t;
  using result_type = uint_type;
  static constexpr size_t word_size = 64;
  static constexpr size_t rotation_a = 24;
  static constexpr size_t shift_b = 16;
  static constexpr size_t rotation_c = 37;

  static constexpr uint_type rotate_left(uint_type x, size_t k) noexcept {
    return (x << k) | (x >> (64 - k));
  }

  xoroshiro128plus() = default;
  xoroshiro128plus(const xoroshiro128plus&) = default;
  xoroshiro128plus& operator=(const xoroshiro128plus&) = default;
  xoroshiro128plus(xoroshiro128plus&&) = default;
  xoroshiro128plus& operator=(xoroshiro128plus&&) = default;

  xoroshiro128plus(uint_type x, uint_type y) : s0{x}, s1{y} {}
  template <typename RNG>
  constexpr explicit xoroshiro128plus(RNG&& rng)
      : s0{(static_cast<uint_type>(rng()) << 32) |
           static_cast<uint_type>(rng())},
        s1{(static_cast<uint_type>(rng()) << 32) |
           static_cast<uint_type>(rng())} {}

  constexpr auto operator()() noexcept;
  constexpr void jump() noexcept;
  constexpr void long_jump() noexcept;
  static constexpr auto min() noexcept { return uint_type{}; }
  static constexpr auto max() noexcept { return ~uint_type{}; }

  uint_type s0{1314472907419283471ul};
  uint_type s1{7870872464127966567ul};
};

constexpr auto xoroshiro128plus::operator()() noexcept {
  // The order is important. Otherwise jumps will not work properly.
  const auto result = s0 + s1;
  s1 ^= s0;
  s0 = rotate_left(s0, rotation_a) ^ s1 ^ (s1 << shift_b);
  s1 = rotate_left(s1, rotation_c);
  return result;
}

constexpr void xoroshiro128plus::jump() noexcept {
  // Magic numbers depend on rotation and shift arguments.
  constexpr uint_type mask[] = {0xdf900294d8f554a5ul, 0x170865df4b3201fcul};
  uint_type result0 = 0;
  uint_type result1 = 0;
  for (int i = 0; i < 2; i++) {
    for (size_t b = 0; b < word_size; b++) {
      // if (mask[i] & (1ul << b)) {
      //   result0 ^= s0;
      //   result1 ^= s1;
      // }
      const auto tmp = (mask[i] & (1ul << b)) ? (~uint_type{}) : (0);
      result0 ^= s0 & tmp;
      result1 ^= s1 & tmp;
      operator()();
    }
  }
  s0 = result0;
  s1 = result1;
}

constexpr void xoroshiro128plus::long_jump() noexcept {
  // Magic numbers depend on rotation and shift arguments.
  constexpr uint_type mask[] = {0xd2a98b26625eee7bul, 0xdddf9b1090aa7ac1ul};
  uint_type result0 = 0;
  uint_type result1 = 0;
  for (int i = 0; i < 2; i++) {
    for (size_t b = 0; b < word_size; b++) {
      if (mask[i] & (1ul << b)) {
        result0 ^= s0;
        result1 ^= s1;
      }
      operator()();
    }
  }
  s0 = result0;
  s1 = result1;
}

using xrsr128p = xoroshiro128plus;

}  // namespace pxart