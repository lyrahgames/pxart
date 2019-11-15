#pragma once

#include <immintrin.h>

namespace pxart {

struct mt19937_simd256 {
  using int_type = uint32_t;
  static constexpr int_type N = 624;
  static constexpr int_type M = 397;
  static constexpr int_type MATRIX_A = 0x9908b0dfUL;
  static constexpr int_type UPPER_MASK = 0x80000000UL;
  static constexpr int_type LOWER_MASK = 0x7fffffffUL;
  // static constexpr int_type mag01[2] = {0x0UL, MATRIX_A};

  mt19937_simd256() = default;
  template <typename RNG>
  explicit mt19937_simd256(RNG& rng) {
    for (auto i = N; i > 0; --i) state_ptr[N - i] = rng();
  }

  // constexpr void init(int_type s) noexcept {
  //   state[0] = s & 0xffffffffUL;
  //   for (int i = 1; i < N; ++i) {
  //     state[i] = (1812433253UL * (state[i - 1] ^ (state[i - 1] >> 30)) + i);
  //     state[i] &= 0xffffffffUL;
  //   }
  // }

  auto operator()() noexcept {
    if (state_index >= N) {
      // for (int k = 0; k < N - M; ++k) {
      //   y = (state[k] & UPPER_MASK) | (state[k + 1] & LOWER_MASK);
      //   state[k] = state[k + M] ^ (y >> 1) ^ mag01[y & 0x1UL];
      // }

      const auto transform = [this](int k) {
        const auto tmp = _mm256_or_si256(
            _mm256_and_si256(
                _mm256_loadu_si256(reinterpret_cast<__m256i*>(state_ptr + k)),
                _mm256_set1_epi32(UPPER_MASK)),
            _mm256_and_si256(_mm256_loadu_si256(
                                 reinterpret_cast<__m256i*>(state_ptr + k + 1)),
                             _mm256_set1_epi32(LOWER_MASK)));
        const auto mag01 = _mm256_and_si256(
            _mm256_set1_epi32(MATRIX_A),
            _mm256_cmpgt_epi32(_mm256_and_si256(tmp, _mm256_set1_epi32(1)),
                               _mm256_setzero_si256()));
        const auto tmp2 = _mm256_xor_si256(_mm256_srli_epi32(tmp, 1), mag01);
        return tmp2;
      };

      {
        const auto load =
            _mm256_loadu_si256(reinterpret_cast<__m256i*>(state_ptr + 0 + M));
        const auto tmp2 = transform(0);
        const auto result = _mm256_xor_si256(load, tmp2);
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(state_ptr + 0), result);
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(state_ptr + N), result);
      }

      for (int k = 8; k < N - M - ((N - M) % 8); k += 8) {
        const auto load =
            _mm256_loadu_si256(reinterpret_cast<__m256i*>(state_ptr + k + M));
        const auto tmp2 = transform(k);
        const auto result = _mm256_xor_si256(load, tmp2);
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(state_ptr + k), result);
      }

      {
        // const int k = ((N - M) / 8) * 8;
        const int k = 224;
        const auto load1 =
            _mm256_loadu_si256(reinterpret_cast<__m256i*>(state_ptr + k + M));
        const auto load2 =
            _mm256_loadu_si256(reinterpret_cast<__m256i*>(state + 5));
        const auto tmp2 = transform(k);
        const auto load = _mm256_blend_epi32(load1, load2, 0b11111000);
        const auto result = _mm256_xor_si256(load, tmp2);
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(state_ptr + k), result);
      }

      // for (int k = N - M; k < N - 1; ++k) {
      //   y = (state_ptr[k] & UPPER_MASK) | (state_ptr[k + 1] & LOWER_MASK);
      //   state_ptr[k] = state_ptr[k + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
      // }
      // y = (state_ptr[N - 1] & UPPER_MASK) | (state_ptr[0] & LOWER_MASK);
      // state_ptr[N - 1] = state_ptr[M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

      // for (int k = ((N - M) / 8 + 1) * 8; k < N; ++k) {
      for (int k = 232; k < N; k += 8) {
        const auto tmp2 = transform(k);
        const auto load = _mm256_loadu_si256(
            reinterpret_cast<__m256i*>(state_ptr + (k + M - N)));
        const auto result = _mm256_xor_si256(load, tmp2);
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(state_ptr + k), result);
      }

      state_index = 0;
    }

    // y ^= (y >> 11);
    // y ^= (y << 7) & 0x9d2c5680UL;
    // y ^= (y << 15) & 0xefc60000UL;
    // y ^= (y >> 18);
    auto y =
        _mm256_loadu_si256(reinterpret_cast<__m256i*>(&state_ptr[state_index]));
    state_index += 8;
    y = _mm256_xor_si256(y, _mm256_srli_epi32(y, 11));
    y = _mm256_xor_si256(y, _mm256_and_si256(_mm256_slli_epi32(y, 7),
                                             _mm256_set1_epi32(0x9d2c5680)));
    y = _mm256_xor_si256(y, _mm256_and_si256(_mm256_slli_epi32(y, 15),
                                             _mm256_set1_epi32(0xefc60000)));
    y = _mm256_xor_si256(y, _mm256_srli_epi32(y, 18));
    return y;
  }

  uint32_t state[8 + N + 8];
  uint32_t* state_ptr = &state[8];
  int state_index = N + 1;
};

}  // namespace pxart