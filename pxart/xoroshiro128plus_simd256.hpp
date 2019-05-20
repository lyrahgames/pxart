#pragma once

#include <immintrin.h>

namespace rng {

template <int A, int B, int C>
struct xoroshiro128plus_simd256 {
  using result_type = __m256i;
  static constexpr int parameters[3] = {A, B, C};

  static inline auto rotate_left(__m256i x, int k) noexcept {
    return _mm256_or_si256(_mm256_slli_epi64(x, k),
                           _mm256_srli_epi64(x, 64 - k));
  }

  constexpr result_type operator()() noexcept {
    const auto s0 = state[0];
    const auto s1 = state[1];
    state[1] = _mm256_xor_si256(state[0], state[1]);
    state[0] = _mm256_xor_si256(
        state[1], _mm256_xor_si256(_mm256_slli_epi64(state[1], parameters[1]),
                                   rotate_left(state[0], parameters[0])));
    state[1] = rotate_left(state[1], parameters[2]);
    return _mm256_add_epi64(state[0], state[1]);
  }

  __m256i state[2];
};

using xs128p_simd256 = xoroshiro128plus_simd256<24, 16, 37>;

}  // namespace rng