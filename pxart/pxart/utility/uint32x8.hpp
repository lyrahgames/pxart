#pragma once

#include <pxart/utility/bool1x256.hpp>

namespace pxart::simd256 {

struct uint32x8 : bool1x256 {
  using value_type = uint32_t;

  using bool1x256::bool1x256;

  struct reverse_t {};
  static constexpr reverse_t reverse{};

  uint32x8(value_type init) noexcept : bool1x256{_mm256_set1_epi32(init)} {}
  uint32x8(value_type x0, value_type x1, value_type x2, value_type x3,
           value_type x4, value_type x5, value_type x6, value_type x7) noexcept
      : bool1x256{_mm256_setr_epi32(x0, x1, x2, x3, x4, x5, x6, x7)} {}
  uint32x8(value_type x7, value_type x6, value_type x5, value_type x4,
           value_type x3, value_type x2, value_type x1, value_type x0,
           reverse_t) noexcept
      : bool1x256{_mm256_set_epi32(x7, x6, x5, x4, x3, x2, x1, x0)} {}
};

inline uint32x8 operator+(uint32x8 x, uint32x8 y) noexcept {
  return _mm256_add_epi32(x, y);
}

inline uint32x8 operator+(uint32x8 x) noexcept { return x; }

inline uint32x8 operator-(uint32x8 x, uint32x8 y) noexcept {
  return _mm256_sub_epi32(x, y);
}

inline uint32x8 operator-(uint32x8 x) noexcept { return 0u - x; }

inline uint32x8 hadd(uint32x8 x, uint32x8 y) noexcept {
  return _mm256_hadd_epi32(x, y);
}

inline uint32x8 hsub(uint32x8 x, uint32x8 y) noexcept {
  return _mm256_hsub_epi32(x, y);
}

inline uint32x8 min(uint32x8 x, uint32x8 y) noexcept {
  return _mm256_min_epu32(x, y);
}

inline uint32x8 max(uint32x8 x, uint32x8 y) noexcept {
  return _mm256_max_epu32(x, y);
}

inline uint32x8 operator*(uint32x8 x, uint32x8 y) noexcept {
  return _mm256_mullo_epi32(x, y);
}

inline uint32x8 operator<<(uint32x8 x, int shift) noexcept {
  return _mm256_slli_epi32(x, shift);
}

inline uint32x8 operator<<(uint32x8 x, uint32x8 shift) noexcept {
  return _mm256_sllv_epi32(x, shift);
}

inline uint32x8 operator>>(uint32x8 x, int shift) noexcept {
  return _mm256_srli_epi32(x, shift);
}

inline uint32x8 operator>>(uint32x8 x, uint32x8 shift) noexcept {
  return _mm256_srlv_epi32(x, shift);
}

}  // namespace pxart::simd256