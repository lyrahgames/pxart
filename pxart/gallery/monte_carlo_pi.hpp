#pragma once
#include <immintrin.h>

#include <pxart/simd128/uniform.hpp>
#include <pxart/simd256/uniform.hpp>
#include <pxart/uniform.hpp>
#include <random>

namespace pxart::gallery {

template <typename Real, typename Integer, typename RNG>
inline Real monte_carlo_pi(RNG& rng, Integer samples) noexcept {
  // std::uniform_real_distribution<Real> dist{0, 1};
  Integer samples_in_circle{};
  for (auto i = samples; i > 0; --i) {
    // const auto x = dist(rng);
    // const auto y = dist(rng);
    const auto x = pxart::uniform<Real>(rng);
    const auto y = pxart::uniform<Real>(rng);
    samples_in_circle += (x * x + y * y <= 1);
  }
  return static_cast<Real>(samples_in_circle) / samples * 4;
}

template <typename Real, typename Integer, typename RNG1, typename RNG2>
inline Real monte_carlo_pi(RNG1& rng1, RNG2& rng2, Integer samples) noexcept {
  std::uniform_real_distribution<Real> dist{0, 1};
  Integer samples_in_circle{};
  for (auto i = samples; i > 0; --i) {
    // const auto x = dist(rng1);
    // const auto y = dist(rng2);
    const auto x = pxart::uniform<float>(rng1);
    const auto y = pxart::uniform<float>(rng2);
    samples_in_circle += (x * x + y * y <= 1);
  }
  return static_cast<Real>(samples_in_circle) / samples * 4;
}

namespace simd256::sprng {

template <typename Integer, typename RNG>
inline float monte_carlo_pi(RNG& rng, Integer samples) noexcept {
  std::uniform_real_distribution<float> dist{0, 1};

  auto samples_in_circle = _mm256_setzero_si256();

  for (auto i = samples; i > 0; i -= 8) {
    const auto x =
        _mm256_set_ps(pxart::uniform<float>(rng), pxart::uniform<float>(rng),
                      pxart::uniform<float>(rng), pxart::uniform<float>(rng),
                      pxart::uniform<float>(rng), pxart::uniform<float>(rng),
                      pxart::uniform<float>(rng), pxart::uniform<float>(rng));
    const auto y =
        _mm256_set_ps(pxart::uniform<float>(rng), pxart::uniform<float>(rng),
                      pxart::uniform<float>(rng), pxart::uniform<float>(rng),
                      pxart::uniform<float>(rng), pxart::uniform<float>(rng),
                      pxart::uniform<float>(rng), pxart::uniform<float>(rng));
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

template <typename Integer, typename RNG1, typename RNG2>
inline float monte_carlo_pi(RNG1& rng1, RNG2& rng2, Integer samples) noexcept {
  std::uniform_real_distribution<float> dist{0, 1};

  auto samples_in_circle = _mm256_setzero_si256();

  for (auto i = samples; i > 0; i -= 8) {
    const auto x =
        _mm256_set_ps(dist(rng1), dist(rng1), dist(rng1), dist(rng1),
                      dist(rng1), dist(rng1), dist(rng1), dist(rng1));
    const auto y =
        _mm256_set_ps(dist(rng2), dist(rng2), dist(rng2), dist(rng2),
                      dist(rng2), dist(rng2), dist(rng2), dist(rng2));
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

}  // namespace simd256::sprng

namespace simd256::vprng {

template <typename Integer, typename RNG>
inline float monte_carlo_pi(RNG& rng, Integer samples) noexcept {
  // const auto uniform = [](__m256i x) {
  //   const auto tmp = _mm256_srli_epi32(x, 9);
  //   const auto tmp2 = _mm256_or_si256(tmp, _mm256_set1_epi32(0x3f800000));
  //   return _mm256_sub_ps(_mm256_castsi256_ps(tmp2), _mm256_set1_ps(1.0f));
  // };

  auto samples_in_circle = _mm256_setzero_si256();

  for (auto i = samples; i > 0; i -= 8) {
    // const auto x = uniform(rng());
    // const auto y = uniform(rng());
    const auto x = pxart::simd256::uniform<float>(rng);
    const auto y = pxart::simd256::uniform<float>(rng);
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

template <typename Integer, typename RNG1, typename RNG2>
inline float monte_carlo_pi(RNG1& rng1, RNG2& rng2, Integer samples) noexcept {
  const auto uniform = [](__m256i x) {
    const auto tmp = _mm256_srli_epi32(x, 9);
    const auto tmp2 = _mm256_or_si256(tmp, _mm256_set1_epi32(0x3f800000));
    return _mm256_sub_ps(_mm256_castsi256_ps(tmp2), _mm256_set1_ps(1.0f));
  };

  auto samples_in_circle = _mm256_setzero_si256();

  for (auto i = samples; i > 0; i -= 8) {
    const auto x = uniform(rng1());
    const auto y = uniform(rng2());
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

}  // namespace simd256::vprng

namespace simd128::vprng {

template <typename Integer, typename RNG>
inline float monte_carlo_pi(RNG& rng, Integer samples) noexcept {
  // const auto uniform = [](__m128i x) {
  //   const auto tmp = _mm_srli_epi32(x, 9);
  //   const auto tmp2 = _mm_or_si128(tmp, _mm_set1_epi32(0x3f800000));
  //   return _mm_sub_ps(_mm_castsi128_ps(tmp2), _mm_set1_ps(1.0f));
  // };

  auto samples_in_circle = _mm_setzero_si128();

  for (auto i = samples; i > 0; i -= 4) {
    // const auto x = uniform(rng());
    // const auto y = uniform(rng());
    const auto x = pxart::simd128::uniform<float>(rng);
    const auto y = pxart::simd128::uniform<float>(rng);
    const auto radius = _mm_add_ps(_mm_mul_ps(x, x), _mm_mul_ps(y, y));
    const auto mask =
        _mm_castps_si128(_mm_cmp_ps(radius, _mm_set1_ps(1.0f), _CMP_LE_OQ));
    samples_in_circle = _mm_add_epi32(samples_in_circle,
                                      _mm_and_si128(_mm_set1_epi32(1), mask));
  }
  samples_in_circle = _mm_hadd_epi32(samples_in_circle, samples_in_circle);
  samples_in_circle = _mm_hadd_epi32(samples_in_circle, samples_in_circle);
  return 4.0f * reinterpret_cast<uint32_t*>(&samples_in_circle)[0] / samples;
}

}  // namespace simd128::vprng

}  // namespace pxart::gallery