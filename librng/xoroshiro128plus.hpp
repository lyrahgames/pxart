#pragma once

#include <array>
#include <cstdint>

namespace rng {

template <typename UIntType, int A, int B, int C>
struct xoroshiro128plus {
  using uint_type = UIntType;
  static constexpr int parameters[3] = {A, B, C};
  using result_type = uint_type;

  static constexpr uint_type rotate_left(uint_type x, int k) noexcept {
    return (x << k) | (x >> (64 - k));
  }

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

}  // namespace rng