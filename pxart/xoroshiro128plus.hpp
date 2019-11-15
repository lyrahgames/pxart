#pragma once

#include <array>
#include <cstdint>

namespace pxart {

template <typename UIntType, int A, int B, int C>
struct xoroshiro128plus {
  using uint_type = UIntType;
  static constexpr int parameters[3] = {A, B, C};
  using result_type = uint_type;

  static constexpr uint_type rotate_left(uint_type x, int k) noexcept {
    return (x << k) | (x >> (64 - k));
  }

  xoroshiro128plus() = default;
  template <typename RNG>
  explicit xoroshiro128plus(RNG& rng)
      : s{(static_cast<uint_type>(rng()) << 32) | static_cast<uint_type>(rng()),
          (static_cast<uint_type>(rng()) << 32) |
              static_cast<uint_type>(rng())} {}
  xoroshiro128plus(uint_type x, uint_type y) : s{x, y} {}
  xoroshiro128plus(uint32_t x, uint32_t y, uint32_t z, uint32_t w)
      : s{(static_cast<uint64_t>(y) << 32) | x,
          (static_cast<uint64_t>(w) << 32) | z} {}

  constexpr result_type operator()() noexcept {
    s[1] ^= s[0];
    s[0] = rotate_left(s[0], parameters[0]) ^ s[1] ^ (s[1] << parameters[1]);
    s[1] = rotate_left(s[1], parameters[2]);
    return s[0] + s[1];
  }

  static constexpr result_type min() noexcept { return 0; }
  static constexpr result_type max() noexcept { return -1; }

  uint_type s[2]{1314472907419283471ul, 7870872464127966567ul};
};

using xs128p = xoroshiro128plus<std::uint_fast64_t, 24, 16, 37>;
using xs128p_2016 = xoroshiro128plus<std::uint_fast64_t, 55, 14, 36>;

}  // namespace pxart