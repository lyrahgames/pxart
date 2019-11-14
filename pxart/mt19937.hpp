#pragma once

#include <cstdint>

namespace pxart {

struct mt19937 {
  using int_type = uint64_t;
  static constexpr int_type N = 624;
  static constexpr int_type M = 397;
  static constexpr int_type MATRIX_A = 0x9908b0dfUL;
  static constexpr int_type UPPER_MASK = 0x80000000UL;
  static constexpr int_type LOWER_MASK = 0x7fffffffUL;
  static constexpr int_type mag01[2] = {0x0UL, MATRIX_A};

  mt19937() = default;
  template <typename RNG>
  explicit mt19937(RNG& rng) {
    for (auto i = N; i > 0; --i) state[N - i] = rng();
  }

  constexpr void init(int_type s) noexcept {
    state[0] = s & 0xffffffffUL;
    for (int i = 1; i < N; ++i) {
      state[i] = (1812433253UL * (state[i - 1] ^ (state[i - 1] >> 30)) + i);
      state[i] &= 0xffffffffUL;
    }
  }

  constexpr uint32_t operator()() noexcept {
    const auto generator = [](auto y) {
      y ^= (y >> 11);
      y ^= (y << 7) & 0x9d2c5680UL;
      y ^= (y << 15) & 0xefc60000UL;
      y ^= (y >> 18);
      return y;
    };

    if (state_index >= N) {
      int_type y{};
      // if (state_index == N + 1) init(5489);
      for (int k = 0; k < N - M; ++k) {
        y = (state[k] & UPPER_MASK) | (state[k + 1] & LOWER_MASK);
        state[k] = state[k + M] ^ (y >> 1) ^ mag01[y & 0x1UL];
      }
      for (int k = N - M; k < N - 1; ++k) {
        y = (state[k] & UPPER_MASK) | (state[k + 1] & LOWER_MASK);
        state[k] = state[k + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
      }
      y = (state[N - 1] & UPPER_MASK) | (state[0] & LOWER_MASK);
      state[N - 1] = state[M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];
      state_index = 0;
    }

    return generator(state[state_index++]);
  }

  constexpr int_type min() noexcept { return 0; }
  constexpr int_type max() noexcept { return 0xffffffff; }

  int_type state[N];
  int state_index = N + 1;
};

}  // namespace pxart