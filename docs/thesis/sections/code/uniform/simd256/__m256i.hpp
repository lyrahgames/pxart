template <typename Real>
inline auto uniform(__m256i) noexcept = delete;

template <>
inline auto uniform<float>(__m256i x) noexcept {
  const auto tmp = _mm256_srli_epi32(x, 9);
  const auto tmp2 = _mm256_or_si256(tmp, _mm256_set1_epi32(0x3f800000));
  return _mm256_sub_ps(_mm256_castsi256_ps(tmp2), _mm256_set1_ps(1.0f));
};

template <>
inline auto uniform<double>(__m256i x) noexcept {
  const auto tmp = _mm256_srli_epi64(x, 12);
  const auto tmp2 =
      _mm256_or_si256(tmp, _mm256_set1_epi64x(0x3ff0000000000000L));
  return _mm256_sub_pd(_mm256_castsi256_pd(tmp2), _mm256_set1_pd(1.0));
}

template <typename Real>
inline auto uniform(__m256i x, Real a, Real b) noexcept = delete;

template <>
inline auto uniform<float>(__m256i x, float a, float b) noexcept {
  const auto scale = _mm256_set1_ps(b - a);
  const auto offset = _mm256_set1_ps(a);
  const auto rnd = pxart::simd256::detail::uniform<float>(x);
  return _mm256_add_ps(_mm256_mul_ps(scale, rnd), offset);
}

template <>
inline auto uniform<double>(__m256i x, double a, double b) noexcept {
  const auto scale = _mm256_set1_pd(b - a);
  const auto offset = _mm256_set1_pd(a);
  const auto rnd = pxart::simd256::detail::uniform<double>(x);
  return _mm256_add_pd(_mm256_mul_pd(scale, rnd), offset);
}