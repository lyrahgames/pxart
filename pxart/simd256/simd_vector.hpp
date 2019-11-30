#pragma once
#include <immintrin.h>

namespace pxart::simd256 {

struct simd_vector {
  simd_vector() = default;
  simd_vector(const simd_vector&) = default;
  simd_vector& operator=(const simd_vector&) = default;
  simd_vector(simd_vector&&) = default;
  simd_vector& operator=(simd_vector&&) = default;

  simd_vector(__m256i x) : v256{x} {}
  explicit simd_vector(__m256 x) : v256{_mm256_castps_si256(x)} {}
  explicit simd_vector(__m256d x) : v256{_mm256_castpd_si256(x)} {}

  operator __m256i() const { return v256; }
  explicit operator __m256() const { return _mm256_castsi256_ps(v256); }
  explicit operator __m256d() const { return _mm256_castsi256_pd(v256); }

  __m256i v256;
};

inline simd_vector operator&(simd_vector x, simd_vector y) noexcept {
  return _mm256_and_si256(x, y);
}

inline simd_vector operator|(simd_vector x, simd_vector y) noexcept {
  return _mm256_or_si256(x, y);
}

inline simd_vector operator^(simd_vector x, simd_vector y) noexcept {
  return _mm256_xor_si256(x, y);
}

struct int32x8_t : simd_vector {
  using simd_vector::simd_vector;
};

inline int32x8_t abs(int32x8_t x) noexcept { return _mm256_abs_epi32(x); }

inline int32x8_t operator+(int32x8_t x, int32x8_t y) noexcept {
  return _mm256_add_epi32(x, y);
}

inline int32x8_t operator-(int32x8_t x, int32x8_t y) noexcept {
  return _mm256_sub_epi32(x, y);
}

inline int32x8_t operator*(int32x8_t x, int32x8_t y) noexcept {
  return _mm256_mullo_epi32(x, y);
}

inline int32x8_t operator<<(int32x8_t x, int shift) noexcept {
  return _mm256_slli_epi32(x, shift);
}

inline int32x8_t operator>>(int32x8_t x, int shift) noexcept {
  return _mm256_srai_epi32(x, shift);
}

inline int32x8_t operator<<(int32x8_t x, int32x8_t shift) noexcept {
  return _mm256_sllv_epi32(x, shift);
}

inline int32x8_t operator>>(int32x8_t x, int32x8_t shift) noexcept {
  return _mm256_srav_epi32(x, shift);
}

inline int32x8_t operator==(int32x8_t x, int32x8_t y) noexcept {
  return _mm256_cmpeq_epi32(x, y);
}

inline int32x8_t operator>(int32x8_t x, int32x8_t y) noexcept {
  return _mm256_cmpgt_epi32(x, y);
}

struct uint32x8_t : int32x8_t {
  using int32x8_t::int32x8_t;
  // operator simd_vector() { return v256; }
};

inline uint32x8_t operator>>(uint32x8_t x, int shift) noexcept {
  return _mm256_srli_epi32(x, shift);
}

inline uint32x8_t operator>>(uint32x8_t x, uint32x8_t shift) noexcept {
  return _mm256_srlv_epi32(x, shift);
}

}  // namespace pxart::simd256