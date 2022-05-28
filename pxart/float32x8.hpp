#pragma once

#include <immintrin.h>

namespace pxart::simd256 {

struct float32x8 {
  struct reverse_t {};
  static constexpr reverse_t reverse{};
  struct aligned_t {};
  static constexpr aligned_t aligned{};
  struct unaligned_t {};
  static constexpr unaligned_t unaligned{};
  struct undefined_t {};
  static constexpr undefined_t undefined{};

  float32x8() : data{_mm256_setzero_ps()} {}

  explicit float32x8(float init) : data{_mm256_set1_ps(init)} {}
  float32x8(float x0, float x1, float x2, float x3, float x4, float x5,
            float x6, float x7)
      : data{_mm256_setr_ps(x0, x1, x2, x3, x4, x5, x6, x7)} {}
  float32x8(float x7, float x6, float x5, float x4, float x3, float x2,
            float x1, float x0, reverse_t)
      : data{_mm256_set_ps(x7, x6, x5, x4, x3, x2, x1, x0)} {}

  // Make type conversion to and from intrinsic type implicit.
  // This allows to use intrinsic and library methods in the same place without
  // further type casting.
  float32x8(__m256 init) : data{init} {}
  operator __m256() { return data; }

  float32x8(const float init[8], unaligned_t = {})
      : data{_mm256_loadu_ps(init)} {}
  float32x8(const float init[8], aligned_t) : data{_mm256_load_ps(init)} {}

  float32x8(undefined_t) : data{_mm256_undefined_ps()} {}

  float32x8(const float32x8&) = default;
  float32x8& operator=(const float32x8&) = default;
  float32x8(float32x8&&) = default;
  float32x8& operator=(float32x8&&) = default;

  const float32x8& store(float destination[8], unaligned_t = {}) const {
    _mm256_storeu_ps(destination, data);
    return *this;
  }
  const float32x8& store(float destination[8], aligned_t) const {
    _mm256_store_ps(destination, data);
    return *this;
  }

  float& operator[](unsigned int i) {
    return reinterpret_cast<float*>(&data)[i];
  }
  const float& operator[](unsigned int i) const {
    return reinterpret_cast<const float*>(&data)[i];
  }

  __m256 data;
};

inline float32x8 operator+(float32x8 x, float32x8 y) {
  return _mm256_add_ps(x, y);
}

inline float32x8 operator+(float32x8 x) { return x; }

inline float32x8 operator-(float32x8 x, float32x8 y) {
  return _mm256_sub_ps(x, y);
}

inline float32x8 operator-(float32x8 x) { return float32x8{0.0f} - x; }

inline float32x8 operator*(float32x8 x, float32x8 y) {
  return _mm256_mul_ps(x, y);
}

inline float32x8 operator/(float32x8 x, float32x8 y) {
  return _mm256_div_ps(x, y);
}

inline float32x8 addsub(float32x8 x, float32x8 y) {
  return _mm256_addsub_ps(x, y);
}

inline float32x8 operator&(float32x8 x, float32x8 y) {
  return _mm256_and_ps(x, y);
}

inline float32x8 operator|(float32x8 x, float32x8 y) {
  return _mm256_or_ps(x, y);
}

inline float32x8 operator^(float32x8 x, float32x8 y) {
  return _mm256_xor_ps(x, y);
}

inline float32x8 andnot(float32x8 x, float32x8 y) {
  return _mm256_andnot_ps(x, y);
}

inline float32x8 unpack_high_halfs(float32x8 x, float32x8 y) {
  return _mm256_unpackhi_ps(x, y);
}

inline float32x8 unpack_low_halfs(float32x8 x, float32x8 y) {
  return _mm256_unpacklo_ps(x, y);
}

inline float32x8 sqrt(float32x8 x) { return _mm256_sqrt_ps(x); }

inline float32x8 inv_sqrt(float32x8 x) { return _mm256_rsqrt_ps(x); }

inline float32x8 inv(float32x8 x) { return _mm256_rcp_ps(x); }

inline float32x8 duplicate_odd(float32x8 x) { return _mm256_movehdup_ps(x); }

inline float32x8 duplicate_even(float32x8 x) { return _mm256_moveldup_ps(x); }

inline float32x8 floor(float32x8 x) { return _mm256_floor_ps(x); }

inline float32x8 ceil(float32x8 x) { return _mm256_ceil_ps(x); }

inline float32x8 max(float32x8 x, float32x8 y) { return _mm256_max_ps(x, y); }

inline float32x8 min(float32x8 x, float32x8 y) { return _mm256_min_ps(x, y); }

inline float32x8 hadd(float32x8 x, float32x8 y) { return _mm256_hadd_ps(x, y); }

inline float32x8 hsub(float32x8 x, float32x8 y) { return _mm256_hsub_ps(x, y); }

template <uint8_t a, uint8_t b, uint8_t c, uint8_t d>
    requires(a < 4) && (b < 4) && (c < 4) &&
    (d < 4)  //
    inline float32x8 inlane_permutation(float32x8 x) {
  return _mm256_permute_ps(x, (d << 6) | (c << 4) | (b << 2) | a);
}

template <uint8_t a, uint8_t b>
    requires(a < 4) && (b < 4)  //
    inline float32x8 lane_permutation(float32x8 x, float32x8 y) {
  return _mm256_permute2f128_ps(x, y, (b << 4) | a);
}

// inline float32x8 inlane_permutation(float32x8 x, uint32x8 i) {
//   return _mm256_permutevar_ps(x, i);
// }

// inline float32x8 permutation(float32x8 x, uint32x8 i) {
//   return _m256_permutevar8x32_ps(x, i);
// }

}  // namespace pxart::simd256