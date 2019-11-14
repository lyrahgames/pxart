#pragma once

#include <immintrin.h>

namespace pxart {

template <int A, int B, int C>
struct xoroshiro128plus_simd256 {
  using result_type = __m256i;
  static constexpr int parameters[3] = {A, B, C};

  static inline auto rotate_left(__m256i x, int k) noexcept {
    return _mm256_or_si256(_mm256_slli_epi64(x, k),
                           _mm256_srli_epi64(x, 64 - k));
  }

  xoroshiro128plus_simd256() = default;
  template <typename RNG>
  explicit xoroshiro128plus_simd256(RNG& rng)
      : state{_mm256_set_epi32(rng(), rng(), rng(), rng(), rng(), rng(), rng(),
                               rng()),
              _mm256_set_epi32(rng(), rng(), rng(), rng(), rng(), rng(), rng(),
                               rng())} {}

  constexpr result_type operator()() noexcept {
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

}  // namespace pxart