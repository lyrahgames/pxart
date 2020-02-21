#pragma once
#ifndef __SSE4_1__
#else
#define PXART_SUPPORT_SIMD128_UNIFORM

#include <emmintrin.h>

namespace pxart::simd128 {

namespace detail {

// We introduce this layer of indirection because the Intel compiler is not able
// to deduce the full specialization with automatically deduced return types.
template <typename T>
struct sse_type {};
template <>
struct sse_type<float> {
  using type = __m128;
};
template <>
struct sse_type<double> {
  using type = __m128d;
};
template <typename T>
using sse_t = typename sse_type<T>::type;

template <typename Real>
inline sse_t<Real> uniform(__m128i) noexcept = delete;

template <>
inline sse_t<float> uniform<float>(__m128i x) noexcept {
  const auto tmp = _mm_srli_epi32(x, 9);
  const auto tmp2 = _mm_or_si128(tmp, _mm_set1_epi32(0x3f800000));
  return _mm_sub_ps(_mm_castsi128_ps(tmp2), _mm_set1_ps(1.0f));
}

template <>
inline sse_t<double> uniform<double>(__m128i x) noexcept {
  const auto tmp = _mm_srli_epi64(x, 12);
  const auto tmp2 = _mm_or_si128(tmp, _mm_set1_epi64x(0x3ff0000000000000L));
  return _mm_sub_pd(_mm_castsi128_pd(tmp2), _mm_set1_pd(1.0));
}

template <typename Real>
inline sse_t<Real> uniform(__m128i x, Real a, Real b) noexcept = delete;

template <>
inline sse_t<float> uniform<float>(__m128i x, float a, float b) noexcept {
  const auto scale = _mm_set1_ps(b - a);
  const auto offset = _mm_set1_ps(a);
  const auto rnd = pxart::simd128::detail::uniform<float>(x);
  return _mm_add_ps(_mm_mul_ps(scale, rnd), offset);
}

template <>
inline sse_t<double> uniform<double>(__m128i x, double a, double b) noexcept {
  const auto scale = _mm_set1_pd(b - a);
  const auto offset = _mm_set1_pd(a);
  const auto rnd = pxart::simd128::detail::uniform<double>(x);
  return _mm_add_pd(_mm_mul_pd(scale, rnd), offset);
}

}  // namespace detail

template <typename Real, typename RNG>
constexpr inline auto uniform(RNG&& rng) noexcept {
  return pxart::simd128::detail::uniform<Real>(std::forward<RNG>(rng)());
}

template <typename Real, typename RNG>
constexpr inline auto uniform(RNG&& rng, Real a, Real b) noexcept {
  return pxart::simd128::detail::uniform(std::forward<RNG>(rng)(), a, b);
}

}  // namespace pxart::simd128

#endif