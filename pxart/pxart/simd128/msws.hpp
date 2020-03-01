#pragma once
#ifndef __SSE4_1__
#else
#define PXART_SUPPORT_SIMD128_MSWS

#include <smmintrin.h>

namespace pxart::simd128 {

struct msws {
  using uint_type = uint64_t;
  using result_type = uint32_t;
  using simd_type = __m128i;
  static constexpr size_t simd_size = sizeof(simd_type) / sizeof(result_type);

  static simd_type _mm_square_epi64(simd_type x) noexcept;
  simd_type operator()() noexcept;

  template <typename RNG>
  static constexpr uint_type seed(RNG&& rng) {
    return (static_cast<uint_type>(rng()) << 32) | (rng() << 1) | 0x01;
  }
  template <typename RNG>
  explicit msws(RNG&& rng)
      : step{_mm_set_epi64x(seed(rng), seed(rng)),
             _mm_set_epi64x(seed(rng), seed(rng))},
        root{_mm_setzero_si128(), _mm_setzero_si128()},
        weyl{_mm_setzero_si128(), _mm_setzero_si128()} {}

  simd_type step[2];
  simd_type root[2];
  simd_type weyl[2];
};

inline auto msws::_mm_square_epi64(simd_type x) noexcept -> simd_type {
  // x = x1 * 2^32 + x_0
  // x^2 = 2 * x_1 * x_2 * 2^32 + x_0^2
  const auto first = _mm_mul_epu32(x, x);
  const auto second = _mm_mullo_epi32(x, _mm_slli_epi64(x, 33));
  return _mm_add_epi64(first, second);
}

inline auto msws::operator()() noexcept -> simd_type {
  simd_type result[2];

  for (int i = 0; i < 2; ++i) {
    root[i] = _mm_square_epi64(root[i]);
    weyl[i] = _mm_add_epi64(weyl[i], step[i]);
    root[i] = _mm_add_epi64(root[i], weyl[i]);
    root[i] =
        _mm_or_si128(_mm_srli_epi64(root[i], 32), _mm_slli_epi64(root[i], 32));
    result[i] = root[i];
  }

  return _mm_blend_epi16(_mm_shuffle_epi32(result[0], 0b11'01'10'00),
                         _mm_shuffle_epi32(result[1], 0b10'00'11'01),
                         0b11110000);
  // return _mm_or_si128(
  //     _mm_and_si128(result[0], _mm_set1_epi64x(0xffffffff)),
  //     _mm_slli_epi64(result[1], 32));
}

}  // namespace pxart::simd128

#endif