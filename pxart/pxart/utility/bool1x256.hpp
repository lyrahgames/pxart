#pragma once

#include <immintrin.h>

namespace pxart::simd256 {

struct bool1x256 {
  struct aligned_t {};
  static constexpr aligned_t aligned{};
  struct unaligned_t {};
  static constexpr unaligned_t unaligned{};
  struct undefined_t {};
  static constexpr undefined_t undefined{};

  bool1x256() noexcept : data{_mm256_setzero_si256()} {}
  bool1x256(undefined_t) noexcept : data{_mm256_undefined_si256()} {}

  bool1x256(__m256i init) noexcept : data{init} {}
  operator __m256i() noexcept { return data; }

  bool1x256(const void* init, unaligned_t = {}) noexcept
      : data{_mm256_loadu_si256(reinterpret_cast<const __m256i*>(init))} {}
  bool1x256(const void* init, aligned_t) noexcept
      : data{_mm256_load_si256(reinterpret_cast<const __m256i*>(init))} {}

  bool1x256(const bool1x256&) = default;
  bool1x256& operator=(const bool1x256&) = default;
  bool1x256(bool1x256&&) = default;
  bool1x256& operator=(bool1x256&&) = default;

  __m256i data;
};

inline bool1x256 operator&&(bool1x256 x, bool1x256 y) noexcept {
  return _mm256_and_si256(x, y);
}

inline bool1x256 operator&(bool1x256 x, bool1x256 y) noexcept { return x && y; }

inline bool1x256 andnot(bool1x256 x, bool1x256 y) noexcept {
  return _mm256_andnot_si256(x, y);
}

inline bool1x256 operator||(bool1x256 x, bool1x256 y) noexcept {
  return _mm256_or_si256(x, y);
}

inline bool1x256 operator|(bool1x256 x, bool1x256 y) noexcept { return x || y; }

// inline bool1x256 operator!(bool1x256 x) noexcept {
//   return _mm256_not_si256(x, y);
// }

inline bool1x256 operator^(bool1x256 x, bool1x256 y) noexcept {
  return _mm256_xor_si256(x, y);
}

inline bool1x256 operator+(bool1x256 x, bool1x256 y) noexcept { return x ^ y; }

}  // namespace pxart::simd256