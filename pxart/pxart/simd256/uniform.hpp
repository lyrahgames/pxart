#pragma once
#include <immintrin.h>

namespace pxart::simd256 {

namespace detail {

// We introduce this layer of indirection because the Intel compiler is not able
// to deduce the full specialization with automatically deduced return types.
template <typename T>
struct avx_type {};
template <>
struct avx_type<float> {
  using type = __m256;
};
template <>
struct avx_type<double> {
  using type = __m256d;
};
template <typename T>
using avx_t = typename avx_type<T>::type;

template <typename Real,
          std::enable_if_t<std::is_floating_point_v<Real>, int> = 0>
inline avx_t<Real> uniform(__m256i) noexcept = delete;

template <>
inline avx_t<float> uniform<float>(__m256i x) noexcept {
  const auto tmp = _mm256_srli_epi32(x, 9);
  const auto tmp2 = _mm256_or_si256(tmp, _mm256_set1_epi32(0x3f800000));
  return _mm256_sub_ps(_mm256_castsi256_ps(tmp2), _mm256_set1_ps(1.0f));
};

template <>
inline avx_t<double> uniform<double>(__m256i x) noexcept {
  const auto tmp = _mm256_srli_epi64(x, 12);
  const auto tmp2 =
      _mm256_or_si256(tmp, _mm256_set1_epi64x(0x3ff0000000000000L));
  return _mm256_sub_pd(_mm256_castsi256_pd(tmp2), _mm256_set1_pd(1.0));
}

template <typename Real,
          std::enable_if_t<std::is_floating_point_v<Real>, int> = 0>
inline avx_t<Real> uniform(__m256i x, Real a, Real b) noexcept = delete;

template <>
inline avx_t<float> uniform<float>(__m256i x, float a, float b) noexcept {
  const auto scale = _mm256_set1_ps(b - a);
  const auto offset = _mm256_set1_ps(a);
  const auto rnd = pxart::simd256::detail::uniform<float>(x);
  return _mm256_add_ps(_mm256_mul_ps(scale, rnd), offset);
}

template <>
inline avx_t<double> uniform<double>(__m256i x, double a, double b) noexcept {
  const auto scale = _mm256_set1_pd(b - a);
  const auto offset = _mm256_set1_pd(a);
  const auto rnd = pxart::simd256::detail::uniform<double>(x);
  return _mm256_add_pd(_mm256_mul_pd(scale, rnd), offset);
}

template <typename Integer,
          std::enable_if_t<std::is_integral_v<Integer> &&
                               (sizeof(Integer) <= sizeof(uint32_t)),
                           int> = 0>
inline auto uniform(__m256i x, Integer a, Integer b) noexcept {
  const auto sv = _mm256_set1_epi32(b - a);
  const auto x1 = _mm256_srli_epi32(x, 16);
  const auto x0 = _mm256_and_si256(x, _mm256_set1_epi32(0x0000ffff));
  const auto s1 = _mm256_srli_epi32(sv, 16);
  const auto s0 = _mm256_and_si256(sv, _mm256_set1_epi32(0x0000ffff));

  const auto x1s1 = _mm256_mullo_epi32(x1, s1);
  const auto x1s0 = _mm256_mullo_epi32(x1, s0);
  const auto x0s1 = _mm256_mullo_epi32(x0, s1);
  const auto x0s0 = _mm256_mullo_epi32(x0, s0);
  const auto x1s0_0 = _mm256_and_si256(x1s0, _mm256_set1_epi32(0x0000ffff));
  const auto x1s0_1 = _mm256_srli_epi32(x1s0, 16);
  const auto x0s1_0 = _mm256_and_si256(x0s1, _mm256_set1_epi32(0x0000ffff));
  const auto x0s1_1 = _mm256_srli_epi32(x0s1, 16);
  return _mm256_add_epi32(
      _mm256_set1_epi32(a),
      _mm256_add_epi32(
          x1s1, _mm256_add_epi32(
                    _mm256_add_epi32(x1s0_1, x0s1_1),
                    _mm256_srli_epi32(
                        _mm256_add_epi32(_mm256_add_epi32(x1s0_0, x0s1_0),
                                         _mm256_srli_epi32(x0s0, 16)),
                        16))));
}

}  // namespace detail

template <typename Real, typename RNG>
constexpr inline auto uniform(RNG&& rng) noexcept {
  return pxart::simd256::detail::uniform<Real>(std::forward<RNG>(rng)());
}

template <typename Real, typename RNG>
constexpr inline auto uniform(RNG&& rng, Real a, Real b) noexcept {
  return pxart::simd256::detail::uniform(std::forward<RNG>(rng)(), a, b);
}

}  // namespace pxart::simd256