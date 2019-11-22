#pragma once
#include <immintrin.h>

namespace pxart::simd256 {

inline __m256i _mm256_mullo_epi64(__m256i x, __m256i y) noexcept {
  const auto swap = _mm256_shuffle_epi32(y, 0b10'11'00'01);
  const auto tmp =
      _mm256_hadd_epi32(_mm256_mullo_epi32(swap, x), _mm256_setzero_si256());
  const auto high = _mm256_shuffle_epi32(tmp, 0b01'11'00'10);
  const auto low = _mm256_mul_epu32(x, y);
  return _mm256_add_epi64(high, low);
}

}  // namespace pxart::simd256