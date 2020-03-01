template <typename Integer, typename RNG>
inline float monte_carlo_pi(RNG&& rng, Integer samples) noexcept {
  auto samples_in_circle = _mm256_setzero_si256();
  // Use packet of eight values to run over samples.
  for (auto i = samples; i > 0; i -= 8) {
    // Generate random sample in unit square.
    const auto x = pxart::simd256::uniform<float>(rng);
    const auto y = pxart::simd256::uniform<float>(rng);
    // Computer the squared norm of the sample.
    const auto radius = _mm256_add_ps(_mm256_mul_ps(x, x), _mm256_mul_ps(y, y));
    // Evaluate the circle condition and generate a mask.
    const auto mask = _mm256_castps_si256(
        _mm256_cmp_ps(radius, _mm256_set1_ps(1.0f), _CMP_LE_OQ));
    // Increment when sample lies inside circle by using the mask.
    samples_in_circle = _mm256_add_epi32(
        samples_in_circle, _mm256_and_si256(_mm256_set1_epi32(1), mask));
  }
  // Add the values in each half of samples_in_circle.
  samples_in_circle = _mm256_hadd_epi32(samples_in_circle, samples_in_circle);
  samples_in_circle = _mm256_hadd_epi32(samples_in_circle, samples_in_circle);
  // Return result by adding the last two values with a scalar operation.
  return 4.0f *
         (reinterpret_cast<uint32_t*>(&samples_in_circle)[0] +
          reinterpret_cast<uint32_t*>(&samples_in_circle)[4]) /
         samples;
}