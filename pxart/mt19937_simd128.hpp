#pragma once
#include <tmmintrin.h>

namespace pxart {

struct mt19937_simd128 {
  static constexpr int N = 624;
  static constexpr int M = 397;
  static constexpr uint32_t MATRIX_A = 0x9908b0df;
  static constexpr uint32_t UPPER_MASK = 0x80000000;
  static constexpr uint32_t LOWER_MASK = 0x7fffffff;

  template <typename RNG>
  explicit mt19937_simd128(RNG& rng) {
    for (auto i = N; i > 0; --i) state_ptr[N - i] = rng();
  }

  __m128i operator()() noexcept {
    if (index >= N) {
      const auto transform = [this](int k) {
        const auto tmp = _mm_or_si128(
            _mm_and_si128(
                _mm_loadu_si128(reinterpret_cast<__m128i*>(state_ptr + k)),
                _mm_set1_epi32(UPPER_MASK)),
            _mm_and_si128(
                _mm_loadu_si128(reinterpret_cast<__m128i*>(state_ptr + k + 1)),
                _mm_set1_epi32(LOWER_MASK)));
        const auto mag01 =
            _mm_and_si128(_mm_set1_epi32(MATRIX_A),
                          _mm_cmpgt_epi32(_mm_and_si128(tmp, _mm_set1_epi32(1)),
                                          _mm_setzero_si128()));
        const auto tmp2 = _mm_xor_si128(_mm_srli_epi32(tmp, 1), mag01);
        return tmp2;
      };

      {
        const auto load =
            _mm_loadu_si128(reinterpret_cast<__m128i*>(state_ptr + 0 + M));
        const auto tmp2 = transform(0);
        const auto result = _mm_xor_si128(load, tmp2);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(state_ptr + 0), result);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(state_ptr + N), result);
      }

      for (int k = 4; k < N - M; k += 4) {
        const auto load =
            _mm_loadu_si128(reinterpret_cast<__m128i*>(state_ptr + k + M));
        const auto tmp2 = transform(k);
        const auto result = _mm_xor_si128(load, tmp2);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(state_ptr + k), result);
      }

      for (int k = ((N - M) / 4 + 1) * 4; k < N; k += 4) {
        const auto load = _mm_loadu_si128(
            reinterpret_cast<__m128i*>(state_ptr + (k + M - N)));
        const auto tmp2 = transform(k);
        const auto result = _mm_xor_si128(load, tmp2);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(state_ptr + k), result);
      }

      index = 0;
    }

    auto y = _mm_loadu_si128(reinterpret_cast<__m128i*>(&state_ptr[index]));
    y = _mm_xor_si128(y, _mm_srli_epi32(y, 11));
    y = _mm_xor_si128(
        y, _mm_and_si128(_mm_slli_epi32(y, 7), _mm_set1_epi32(0x9d2c5680)));
    y = _mm_xor_si128(
        y, _mm_and_si128(_mm_slli_epi32(y, 15), _mm_set1_epi32(0xefc60000)));
    y = _mm_xor_si128(y, _mm_srli_epi32(y, 18));

    index += 4;
    return y;
  }

  int index = N;
  uint32_t state[N + 8];
  uint32_t* state_ptr = &state[4];
};

}  // namespace pxart