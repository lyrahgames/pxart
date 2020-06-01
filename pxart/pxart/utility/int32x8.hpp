#pragma once

#include <pxart/utility/bool1x256.hpp>

namespace pxart::simd256 {

struct int32x8 : bool1x256 {
  using value_type = int32_t;

  using bool1x256::bool1x256;

  struct reverse_t {};
  static constexpr reverse_t reverse{};

  int32x8(value_type init) noexcept : bool1x256{_mm256_set1_epi32(init)} {}
  int32x8(value_type x0, value_type x1, value_type x2, value_type x3,
          value_type x4, value_type x5, value_type x6, value_type x7) noexcept
      : bool1x256{_mm256_setr_epi32(x0, x1, x2, x3, x4, x5, x6, x7)} {}
  int32x8(value_type x7, value_type x6, value_type x5, value_type x4,
          value_type x3, value_type x2, value_type x1, value_type x0,
          reverse_t) noexcept
      : bool1x256{_mm256_set_epi32(x7, x6, x5, x4, x3, x2, x1, x0)} {}
};

inline int32x8 operator&(int32x8 x, int32x8 y) noexcept {
  return _mm256_and_si256(x, y);
}

inline int32x8 andnot(int32x8 x, int32x8 y) noexcept {
  return _mm256_andnot_si256(x, y);
}

inline int32x8 operator|(int32x8 x, int32x8 y) noexcept {
  return _mm256_or_si256(x, y);
}

inline int32x8 operator^(int32x8 x, int32x8 y) noexcept {
  return _mm256_xor_si256(x, y);
}

inline int testz(int32x8 x, int32x8 y) noexcept {
  return _mm256_testz_si256(x, y);
}

inline int testnzc(int32x8 x, int32x8 y) noexcept {
  return _mm256_testnzc_si256(x, y);
}

inline int testc(int32x8 x, int32x8 y) noexcept {
  return _mm256_testc_si256(x, y);
}

inline int32x8 operator+(int32x8 x, int32x8 y) noexcept {
  return _mm256_add_epi32(x, y);
}

inline int32x8 operator+(int32x8 x) noexcept { return x; }

inline int32x8 operator-(int32x8 x, int32x8 y) noexcept {
  return _mm256_sub_epi32(x, y);
}

inline int32x8 operator-(int32x8 x) noexcept { return 0 - x; }

inline int32x8 hadd(int32x8 x, int32x8 y) noexcept {
  return _mm256_hadd_epi32(x, y);
}

inline int32x8 hsub(int32x8 x, int32x8 y) noexcept {
  return _mm256_hsub_epi32(x, y);
}

inline int32x8 min(int32x8 x, int32x8 y) noexcept {
  return _mm256_min_epi32(x, y);
}

inline int32x8 max(int32x8 x, int32x8 y) noexcept {
  return _mm256_max_epi32(x, y);
}

inline int32x8 operator*(int32x8 x, int32x8 y) noexcept {
  return _mm256_mullo_epi32(x, y);
}

inline int32x8 operator<<(int32x8 x, int shift) noexcept {
  return _mm256_slli_epi32(x, shift);
}

inline int32x8 operator<<(int32x8 x, int32x8 shift) noexcept {
  return _mm256_sllv_epi32(x, shift);
}

inline int32x8 operator>>(int32x8 x, int shift) noexcept {
  return _mm256_srai_epi32(x, shift);
}

inline int32x8 operator>>(int32x8 x, int32x8 shift) noexcept {
  return _mm256_srav_epi32(x, shift);
}

}  // namespace pxart::simd256