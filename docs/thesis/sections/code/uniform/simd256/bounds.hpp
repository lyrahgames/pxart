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