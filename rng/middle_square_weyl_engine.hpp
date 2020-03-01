#pragma once

#include <immintrin.h>

#include <cstdint>

namespace pxart {

struct middle_square_weyl_engine {
  using result_type = uint32_t;

  middle_square_weyl_engine() = default;

  template <typename RNG>
  middle_square_weyl_engine(RNG& rng)
      : seed{(static_cast<uint64_t>(rng()) << 32) | (rng() << 1) | 0x01} {}

  constexpr result_type operator()() noexcept {
    state *= state;
    weyl += seed;
    state += weyl;
    state = (state >> 32) | (state << 32);
    return state;
  }
  static constexpr result_type min() noexcept { return 0; }
  static constexpr result_type max() noexcept { return 0xffffffff; }

  uint64_t seed = 0xb5ad4eceda1ce2a9;
  uint64_t state = 0;
  uint64_t weyl = 0;
};

namespace simd256 {

struct msws {
  template <typename RNG>
  static constexpr uint64_t seed(RNG& rng) {
    return (static_cast<uint64_t>(rng()) << 32) | (rng() << 1) | 0x01;
  }

  static inline __m256i _mm256_mul_epi64(__m256i x, __m256i y) noexcept {
    const auto swap = _mm256_shuffle_epi32(y, 0b10'11'00'01);
    const auto tmp =
        _mm256_hadd_epi32(_mm256_mullo_epi32(swap, x), _mm256_setzero_si256());
    const auto high = _mm256_shuffle_epi32(tmp, 0b01'11'00'10);
    const auto low = _mm256_mul_epu32(x, y);
    return _mm256_add_epi64(high, low);
  }

  static inline __m256i _mm256_square_epi64(__m256i x) noexcept {
    // x = x1 * 2^32 + x_0
    // x^2 = 2 * x_1 * x_2 * 2^32 + x_0^2
    const auto first = _mm256_mul_epu32(x, x);
    const auto second = _mm256_mullo_epi32(x, _mm256_slli_epi64(x, 33));
    return _mm256_add_epi64(first, second);
  }

  template <typename RNG>
  explicit msws(RNG& rng)
      : step{_mm256_set_epi64x(seed(rng), seed(rng), seed(rng), seed(rng)),
             _mm256_set_epi64x(seed(rng), seed(rng), seed(rng), seed(rng))},
        root{_mm256_setzero_si256(), _mm256_setzero_si256()},
        weyl{_mm256_setzero_si256(), _mm256_setzero_si256()} {}

  auto operator()() noexcept {
    __m256i result[2];

    for (int i = 0; i < 2; ++i) {
      root[i] = _mm256_square_epi64(root[i]);
      weyl[i] = _mm256_add_epi64(weyl[i], step[i]);
      root[i] = _mm256_add_epi64(root[i], weyl[i]);
      root[i] = _mm256_or_si256(_mm256_srli_epi64(root[i], 32),
                                _mm256_slli_epi64(root[i], 32));
      result[i] = root[i];
    }

    return _mm256_blend_epi32(
        _mm256_permutevar8x32_epi32(result[0],
                                    _mm256_set_epi32(7, 5, 3, 1, 6, 4, 2, 0)),
        _mm256_permutevar8x32_epi32(result[1],
                                    _mm256_set_epi32(6, 4, 2, 0, 7, 5, 3, 1)),
        0b11110000);
    // return _mm256_or_si256(
    //     _mm256_and_si256(result[0], _mm256_set1_epi64x(0xffffffff)),
    //     _mm256_slli_epi64(result[1], 32));
  }

  __m256i step[2];
  __m256i root[2];
  __m256i weyl[2];
};

}  // namespace simd256

namespace simd128 {

struct msws {
  template <typename RNG>
  static constexpr uint64_t seed(RNG& rng) {
    return (static_cast<uint64_t>(rng()) << 32) | (rng() << 1) | 0x01;
  }

  static inline __m128i _mm_square_epi64(__m128i x) noexcept {
    // x = x1 * 2^32 + x_0
    // x^2 = 2 * x_1 * x_2 * 2^32 + x_0^2
    const auto first = _mm_mul_epu32(x, x);
    const auto second = _mm_mullo_epi32(x, _mm_slli_epi64(x, 33));
    return _mm_add_epi64(first, second);
  }

  template <typename RNG>
  explicit msws(RNG& rng)
      : step{_mm_set_epi64x(seed(rng), seed(rng)),
             _mm_set_epi64x(seed(rng), seed(rng))},
        root{_mm_setzero_si128(), _mm_setzero_si128()},
        weyl{_mm_setzero_si128(), _mm_setzero_si128()} {}

  auto operator()() noexcept {
    __m128i result[2];

    for (int i = 0; i < 2; ++i) {
      root[i] = _mm_square_epi64(root[i]);
      weyl[i] = _mm_add_epi64(weyl[i], step[i]);
      root[i] = _mm_add_epi64(root[i], weyl[i]);
      root[i] = _mm_or_si128(_mm_srli_epi64(root[i], 32),
                             _mm_slli_epi64(root[i], 32));
      result[i] = root[i];
    }

    return _mm_blend_epi32(_mm_shuffle_epi32(result[0], 0b11'01'10'00),
                           _mm_shuffle_epi32(result[1], 0b10'00'11'01), 0b1100);
    // return _mm256_or_si256(
    //     _mm256_and_si256(result[0], _mm256_set1_epi64x(0xffffffff)),
    //     _mm256_slli_epi64(result[1], 32));
  }

  __m128i step[2];
  __m128i root[2];
  __m128i weyl[2];
};

}  // namespace simd128

}  // namespace pxart