#pragma once
#include <immintrin.h>

#include <array>
#include <cstdint>

namespace pxart {

template <typename Real>
struct uniform_real_distribution {
  using real_type = Real;
  using result_type = real_type;
  using param_type = real_type;

  real_type min() const noexcept { return a; }
  real_type max() const noexcept { return b; }

  template <typename RNG>
  real_type operator()(RNG& rng) const noexcept {
    return (static_cast<real_type>(rng()) - static_cast<real_type>(rng.min())) /
               (static_cast<real_type>(rng.max()) -
                static_cast<real_type>(rng.min()) + 1) *
               (b - a) +
           a;
  }

  real_type a, b;
};

inline float uniform(uint32_t x) noexcept {
  const auto tmp = ((x >> 9) | 0x3f800000);
  return (*reinterpret_cast<const float*>(&tmp)) - 1.0f;
}

inline float uniform(uint32_t x, float a, float b) noexcept {
  return (b - a) * uniform(x) + a;
}

inline __m256 uniform(__m256i x) noexcept {
  const auto tmp = _mm256_srli_epi32(x, 9);
  const auto tmp2 = _mm256_or_si256(tmp, _mm256_set1_epi32(0x3f800000));
  return _mm256_sub_ps(_mm256_castsi256_ps(tmp2), _mm256_set1_ps(1.0f));
};

inline __m256 uniform(__m256i x, float a, float b) noexcept {
  const auto scale = _mm256_set1_ps(b - a);
  const auto offset = _mm256_set1_ps(a);
  const auto rnd = uniform(x);
  return _mm256_add_ps(_mm256_mul_ps(scale, rnd), offset);
}

inline double uniform_double(uint64_t x) noexcept {
  const auto tmp = ((x >> 12) | 0x3ff0000000000000ULL);
  return (*reinterpret_cast<const double*>(&tmp)) - 1.0;
}

inline double uniform_double(uint64_t x, double a, double b) noexcept {
  return (b - a) * x + a;
}

inline double uniform_double(uint32_t x) noexcept {
  const auto tmp = ((static_cast<uint64_t>(x) << 20) | 0x3ff0000000000000ULL);
  return (*reinterpret_cast<const double*>(&tmp)) - 1.0;
}

inline __m256d uniform_double(__m256i x) noexcept {
  const auto tmp = _mm256_srli_epi64(x, 12);
  const auto tmp2 =
      _mm256_or_si256(tmp, _mm256_set1_epi64x(0x3ff0000000000000L));
  return _mm256_sub_pd(_mm256_castsi256_pd(tmp2), _mm256_set1_pd(1.0));
}

inline __m256d uniform_double(__m256i x, double a, double b) noexcept {
  const auto scale = _mm256_set1_pd(b - a);
  const auto offset = _mm256_set1_pd(a);
  const auto rnd = uniform_double(x);
  return _mm256_add_pd(_mm256_mul_pd(scale, rnd), offset);
}

inline std::array<__m256d, 2> uniform_double_from_32(__m256i x) noexcept {
  const auto mask = _mm256_set_epi32(0, -1, 0, -1, 0, -1, 0, -1);
  const auto tmp = _mm256_and_si256(x, mask);
  const auto first = uniform_double(_mm256_sub_epi32(x, tmp));
  const auto second = _mm256_sub_pd(
      _mm256_castsi256_pd(_mm256_or_si256(
          _mm256_slli_epi64(tmp, 20), _mm256_set1_epi64x(0x3ff0000000000000L))),
      _mm256_set1_pd(1.0));
  return {first, second};
}

}  // namespace pxart