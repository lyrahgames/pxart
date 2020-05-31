#pragma once

#include <immintrin.h>

#include <cstdint>

namespace pxart::simd256 {

struct bool1x256 {
  struct aligned_t {};
  static constexpr aligned_t aligned{};
  struct unaligned_t {};
  static constexpr unaligned_t unaligned{};
  struct streamed_t {};
  static constexpr streamed_t streamed{};
  struct cache_line_crossed_t {};
  static constexpr cache_line_crossed_t cache_line_crossed{};
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
  bool1x256(const void* init, streamed_t) noexcept
      : data{_mm256_stream_load_si256(reinterpret_cast<const __m256i*>(init))} {
  }
  bool1x256(const void* init, cache_line_crossed_t) noexcept
      : data{_mm256_lddqu_si256(reinterpret_cast<const __m256i*>(init))} {}

  bool1x256(const bool1x256&) = default;
  bool1x256& operator=(const bool1x256&) = default;
  bool1x256(bool1x256&&) = default;
  bool1x256& operator=(bool1x256&&) = default;

  const bool1x256& store(void* destination, unaligned_t = {}) const noexcept {
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(destination), data);
    return *this;
  }
  const bool1x256& store(void* destination, aligned_t) const noexcept {
    _mm256_store_si256(reinterpret_cast<__m256i*>(destination), data);
    return *this;
  }
  const bool1x256& store(void* destination, streamed_t) const noexcept {
    _mm256_stream_si256(reinterpret_cast<__m256i*>(destination), data);
    return *this;
  }

  // explicit operator uint32_t() noexcept { return _mm256_cvtsi256_si32(data);
  // }

  __m256i data;
};

inline bool1x256 operator&&(bool1x256 x, bool1x256 y) noexcept {
  return _mm256_and_si256(x, y);
}

inline bool1x256 operator&(bool1x256 x, bool1x256 y) noexcept { return x && y; }

// inline bool1x256 operator*(bool1x256 x, bool1x256 y) noexcept { return x & y;
// }

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

// inline bool1x256 operator+(bool1x256 x, bool1x256 y) noexcept { return x ^ y;
// }

// inline bool1x256 operator-(bool1x256 x, bool1x256 y) noexcept { return x + y;
// }

inline int testz(bool1x256 x, bool1x256 y) noexcept {
  return _mm256_testz_si256(x, y);
}

inline int testnzc(bool1x256 x, bool1x256 y) noexcept {
  return _mm256_testnzc_si256(x, y);
}

inline int testc(bool1x256 x, bool1x256 y) noexcept {
  return _mm256_testc_si256(x, y);
}

}  // namespace pxart::simd256