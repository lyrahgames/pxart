#pragma once

#include <pxart/utility/bool1x256.hpp>

namespace pxart::simd256 {

struct uint64x4 : bool1x256 {
  using value_type = uint64_t;

  using bool1x256::bool1x256;

  struct reverse_t {};
  static constexpr reverse_t reverse{};

  uint64x4(value_type init) noexcept : bool1x256{_mm256_set1_epi64x(init)} {}
  uint64x4(value_type x0, value_type x1, value_type x2, value_type x3) noexcept
      : bool1x256{_mm256_setr_epi64x(x0, x1, x2, x3)} {}
  uint64x4(value_type x3, value_type x2, value_type x1, value_type x0,
           reverse_t) noexcept
      : bool1x256{_mm256_set_epi64x(x3, x2, x1, x0)} {}
};

inline uint64x4 operator&(uint64x4 x, uint64x4 y) noexcept {
  return _mm256_and_si256(x, y);
}

inline uint64x4 andnot(uint64x4 x, uint64x4 y) noexcept {
  return _mm256_andnot_si256(x, y);
}

inline uint64x4 operator|(uint64x4 x, uint64x4 y) noexcept {
  return _mm256_or_si256(x, y);
}

inline uint64x4 operator^(uint64x4 x, uint64x4 y) noexcept {
  return _mm256_xor_si256(x, y);
}

inline int testz(uint64x4 x, uint64x4 y) noexcept {
  return _mm256_testz_si256(x, y);
}

inline int testnzc(uint64x4 x, uint64x4 y) noexcept {
  return _mm256_testnzc_si256(x, y);
}

inline int testc(uint64x4 x, uint64x4 y) noexcept {
  return _mm256_testc_si256(x, y);
}

inline uint64x4 operator+(uint64x4 x, uint64x4 y) noexcept {
  return _mm256_add_epi64(x, y);
}

inline uint64x4 operator+(uint64x4 x) noexcept { return x; }

inline uint64x4 operator-(uint64x4 x, uint64x4 y) noexcept {
  return _mm256_sub_epi64(x, y);
}

inline uint64x4 operator-(uint64x4 x) noexcept { return 0u - x; }

inline uint64x4 hadd(uint64x4 x, uint64x4 y) noexcept {
  return _mm256_hadd_epi64(x, y);
}

inline uint64x4 hsub(uint64x4 x, uint64x4 y) noexcept {
  return _mm256_hsub_epi64(x, y);
}

inline uint64x4 min(uint64x4 x, uint64x4 y) noexcept {
  return _mm256_min_epu64(x, y);
}

inline uint64x4 max(uint64x4 x, uint64x4 y) noexcept {
  return _mm256_max_epu64(x, y);
}

// inline uint64x4 operator*(uint64x4 x, uint64x4 y) noexcept {
//   return _mm256_mullo_epi64(x, y);
// }

inline uint64x4 operator<<(uint64x4 x, int shift) noexcept {
  return _mm256_slli_epi64(x, shift);
}

inline uint64x4 operator<<(uint64x4 x, uint64x4 shift) noexcept {
  return _mm256_sllv_epi64(x, shift);
}

inline uint64x4 operator>>(uint64x4 x, int shift) noexcept {
  return _mm256_srli_epi64(x, shift);
}

inline uint64x4 operator>>(uint64x4 x, uint64x4 shift) noexcept {
  return _mm256_srlv_epi64(x, shift);
}

}  // namespace pxart::simd256