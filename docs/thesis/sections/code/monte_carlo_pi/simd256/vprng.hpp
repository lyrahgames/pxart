template <typename Integer, typename RNG>
inline float monte_carlo_pi(RNG& rng, Integer samples) noexcept {
  const auto uniform = [](__m256i x) {
    const auto tmp = _mm256_srli_epi32(x, 9);
    const auto tmp2 = _mm256_or_si256(tmp, _mm256_set1_epi32(0x3f800000));
    return _mm256_sub_ps(_mm256_castsi256_ps(tmp2), _mm256_set1_ps(1.0f));
  };

  auto samples_in_circle = _mm256_setzero_si256();

  for (auto i = samples; i > 0; i -= 8) {
    const auto x = uniform(rng());
    const auto y = uniform(rng());
    const auto radius = _mm256_add_ps(_mm256_mul_ps(x, x), _mm256_mul_ps(y, y));
    const auto mask = _mm256_castps_si256(
        _mm256_cmp_ps(radius, _mm256_set1_ps(1.0f), _CMP_LE_OQ));
    samples_in_circle = _mm256_add_epi32(
        samples_in_circle, _mm256_and_si256(_mm256_set1_epi32(1), mask));
  }
  samples_in_circle = _mm256_hadd_epi32(samples_in_circle, samples_in_circle);
  samples_in_circle = _mm256_hadd_epi32(samples_in_circle, samples_in_circle);
  return 4.0f *
         (reinterpret_cast<uint32_t*>(&samples_in_circle)[0] +
          reinterpret_cast<uint32_t*>(&samples_in_circle)[4]) /
         samples;
}