#pragma once
#ifndef __SSE4_1__
#else
#define PXART_SUPPORT_SIMD128_UNIFORM

// #include <emmintrin.h>
#include <smmintrin.h>
// #include <nmmintrin.h>

#include <pxart/uniform.hpp>

namespace pxart::simd128 {

namespace detail {

// We introduce this layer of indirection because the Intel compiler is not able
// to deduce the full specialization with automatically deduced return types.
template <typename T, typename U = void>
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
struct sse_type<T, std::enable_if_t<std::is_integral_v<T> > > {
  using type = __m128i;
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

template <typename Real,
          std::enable_if_t<std::is_floating_point_v<Real>, int> = 0>
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

template <typename Integer,
          std::enable_if_t<std::is_integral_v<Integer>, int> = 0>
inline auto uniform(__m128i x) noexcept {
  return x;
}

template <typename Integer,
          std::enable_if_t<std::is_integral_v<Integer> &&
                               (sizeof(Integer) == sizeof(uint8_t)),
                           int> = 0>
inline auto uniform(__m128i x, Integer a, Integer b) noexcept {
  const auto bound = _mm_set1_epi16(b - a + 1);
  const auto even = _mm_srli_epi16(
      _mm_mullo_epi16(_mm_and_si128(x, _mm_set1_epi16(0x00ff)), bound), 8);
  const auto odd = _mm_mullo_epi16(_mm_srli_epi16(x, 8), bound);
  const auto scale = _mm_blendv_epi8(odd, even, _mm_set1_epi16(0x00ff));
  return _mm_add_epi8(scale, _mm_set1_epi8(a));
}

template <typename Integer,
          std::enable_if_t<std::is_integral_v<Integer> &&
                               (sizeof(Integer) == sizeof(uint16_t)),
                           int> = 0>
inline auto uniform(__m128i x, Integer a, Integer b) noexcept {
  const auto bound = _mm_set1_epi16(b - a + 1);
  const auto scale = _mm_mulhi_epu16(x, bound);
  return _mm_add_epi16(scale, _mm_set1_epi16(a));
}

template <typename Integer,
          std::enable_if_t<std::is_integral_v<Integer> &&
                               (sizeof(Integer) == sizeof(uint32_t)),
                           int> = 0>
inline auto uniform(__m128i x, Integer a, Integer b) noexcept {
  const auto bound = _mm_set1_epi32(b - a + 1);
  const auto even = _mm_srli_epi64(_mm_mul_epu32(x, bound), 32);
  const auto odd = _mm_mul_epu32(_mm_srli_epi64(x, 32), bound);
  // const auto scale = _mm_blend_epi32(odd, even, 0b01010101);
  const auto scale = _mm_or_si128(
      _mm_and_si128(odd, _mm_set1_epi64x(0xffffffff00000000)), even);
  return _mm_add_epi32(scale, _mm_set1_epi32(a));
}

template <typename Integer,
          std::enable_if_t<std::is_integral_v<Integer> &&
                               (sizeof(Integer) == sizeof(uint64_t)),
                           int> = 0>
inline auto uniform(__m128i x, Integer a, Integer b) noexcept {
  const auto lower_mask = _mm_set1_epi64x(0x00000000ffffffff);
  const auto sv = _mm_set1_epi64x(b - a + 1);

  // uint64_t x, s;
  // uint32_t x1, x0, s1, s0;
  // x = (x1 << 32) + x0
  // s = (s1 << 32) + s0
  // x * s = (x1 * s1 << 64) + ((x1 * s0 + x0 * s1) << 32) + x0 * s0
  // Every sum can introduce an overflow and every multiplication is a 64-bit
  // multiplication with arguments that are only filled in the lower 32 bits.
  const auto x1 = _mm_srli_epi64(x, 32);
  const auto x0 = _mm_and_si128(x, lower_mask);
  const auto s1 = _mm_srli_epi64(sv, 32);
  const auto s0 = _mm_and_si128(sv, lower_mask);

  const auto x1s1 = _mm_mul_epu32(x1, s1);
  const auto x1s0 = _mm_mul_epu32(x1, s0);
  const auto x0s1 = _mm_mul_epu32(x0, s1);
  const auto x0s0 = _mm_mul_epu32(x0, s0);

  const auto x1s0_0 = _mm_and_si128(x1s0, lower_mask);
  const auto x1s0_1 = _mm_srli_epi64(x1s0, 32);
  const auto x0s1_0 = _mm_and_si128(x0s1, lower_mask);
  const auto x0s1_1 = _mm_srli_epi64(x0s1, 32);

  return _mm_add_epi64(
      _mm_set1_epi64x(a),
      _mm_add_epi64(
          x1s1, _mm_add_epi64(
                    _mm_add_epi64(x1s0_1, x0s1_1),
                    _mm_srli_epi64(_mm_add_epi64(_mm_add_epi64(x1s0_0, x0s1_0),
                                                 _mm_srli_epi64(x0s0, 32)),
                                   32))));
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

namespace pxart {

// template <typename Real, typename RNG>
// constexpr inline auto uniform(RNG&& rng) noexcept -> std::enable_if_t<
//     !decltype(has_uniform_01(rng))::value &&
//         std::is_same_v<decltype(rng()), __m128i>,
//     std::conditional_t<std::is_same_v<Real, float>, __m128, __m128d> > {
//   return pxart::simd128::uniform<Real>(std::forward<RNG>(rng));
// }

template <typename T, typename RNG>
constexpr inline auto uniform(RNG&& rng) noexcept
    -> std::enable_if_t<!decltype(has_uniform_01(rng))::value &&
                            std::is_same_v<decltype(rng()), __m128i>,
                        pxart::simd128::detail::sse_t<T> > {
  return pxart::simd128::uniform<T>(std::forward<RNG>(rng));
}

template <typename T, typename RNG>
constexpr inline auto uniform(RNG&& rng, T a, T b) noexcept
    -> std::enable_if_t<!decltype(has_uniform(rng, a, b))::value &&
                            std::is_same_v<decltype(rng()), __m128i>,
                        pxart::simd128::detail::sse_t<T> > {
  return pxart::simd128::uniform<T>(std::forward<RNG>(rng), a, b);
}

}  // namespace pxart

#endif