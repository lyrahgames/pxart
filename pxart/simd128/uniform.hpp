#pragma once
#include <emmintrin.h>

namespace pxart::simd128 {

namespace detail {

template <typename Real>
inline auto uniform(__m128i) noexcept = delete;

template <>
inline auto uniform<float>(__m128i x) noexcept {
  const auto tmp = _mm_srli_epi32(x, 9);
  const auto tmp2 = _mm_or_si128(tmp, _mm_set1_epi32(0x3f800000));
  return _mm_sub_ps(_mm_castsi128_ps(tmp2), _mm_set1_ps(1.0f));
};

template <>
inline auto uniform<double>(__m128i x) noexcept {
  const auto tmp = _mm_srli_epi64(x, 12);
  const auto tmp2 = _mm_or_si128(tmp, _mm_set1_epi64x(0x3ff0000000000000L));
  return _mm_sub_pd(_mm_castsi128_pd(tmp2), _mm_set1_pd(1.0));
}

template <typename Real>
inline auto uniform(__m128i x, Real a, Real b) noexcept = delete;

template <>
inline auto uniform<float>(__m128i x, float a, float b) noexcept {
  const auto scale = _mm_set1_ps(b - a);
  const auto offset = _mm_set1_ps(a);
  const auto rnd = pxart::simd128::detail::uniform<float>(x);
  return _mm_add_ps(_mm_mul_ps(scale, rnd), offset);
}

template <>
inline auto uniform<double>(__m128i x, double a, double b) noexcept {
  const auto scale = _mm_set1_pd(b - a);
  const auto offset = _mm_set1_pd(a);
  const auto rnd = pxart::simd128::detail::uniform<double>(x);
  return _mm_add_pd(_mm_mul_pd(scale, rnd), offset);
}

}  // namespace detail

}  // namespace pxart::simd128