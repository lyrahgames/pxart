#pragma once
#include <smmintrin.h>

#include <cstdint>
#include <iostream>
#include <type_traits>

namespace pxart::simd128 {

struct xoroshiro128plus {
  using uint_type = uint64_t;
  using simd_type = __m128i;
  using result_type = simd_type;
  static constexpr size_t simd_size = 4;
  static constexpr size_t word_size = 64;
  static constexpr size_t rotation_a = 24;
  static constexpr size_t shift_b = 16;
  static constexpr size_t rotation_c = 37;

  static inline auto rotate_left(simd_type x, int k) noexcept {
    return _mm_or_si128(_mm_slli_epi64(x, k), _mm_srli_epi64(x, 64 - k));
  }

  xoroshiro128plus() = default;
  xoroshiro128plus(const xoroshiro128plus& rng) = default;
  xoroshiro128plus& operator=(const xoroshiro128plus&) = default;
  xoroshiro128plus(xoroshiro128plus&&) = default;
  xoroshiro128plus& operator=(xoroshiro128plus&&) = default;

  template <typename RNG>
  explicit xoroshiro128plus(RNG&& rng)
      : s0{_mm_set_epi32(rng(), rng(), rng(), rng())},
        s1{_mm_set_epi32(rng(), rng(), rng(), rng())} {}

  auto operator()() noexcept;
  void jump() noexcept;
  void long_jump() noexcept;
  static constexpr auto min() noexcept { return uint_type{}; }
  static constexpr auto max() noexcept { return ~uint_type{}; }

  simd_type s0;
  simd_type s1;
};

inline auto xoroshiro128plus::operator()() noexcept {
  // The order is important. Otherwise jumps will not work properly.
  const auto result = _mm_add_epi64(s0, s1);
  s1 = _mm_xor_si128(s0, s1);
  s0 = _mm_xor_si128(s1, _mm_xor_si128(_mm_slli_epi64(s1, shift_b),
                                       rotate_left(s0, rotation_a)));
  s1 = rotate_left(s1, rotation_c);
  return result;
}

inline void xoroshiro128plus::jump() noexcept {
  // Magic numbers depend on rotation and shift arguments.
  const simd_type jump_mask[] = {_mm_set1_epi64x(0xdf900294d8f554a5ul),
                                 _mm_set1_epi64x(0x170865df4b3201fcul)};
  const auto zero = _mm_setzero_si128();
  const auto one = _mm_set1_epi64x(1ul);
  auto result0 = zero;
  auto result1 = zero;
  for (int i = 0; i < 2; i++) {
    auto bit = one;
    for (size_t b = 0; b < word_size; ++b) {
      // const auto bit = _mm_slli_epi64(one, b);
      const auto mask = _mm_cmpeq_epi64(_mm_and_si128(jump_mask[i], bit), zero);
      result0 = _mm_xor_si128(result0, _mm_andnot_si128(mask, s0));
      result1 = _mm_xor_si128(result1, _mm_andnot_si128(mask, s1));
      s1 = _mm_xor_si128(s0, s1);
      s0 = _mm_xor_si128(s1, _mm_xor_si128(_mm_slli_epi64(s1, shift_b),
                                           rotate_left(s0, rotation_a)));
      s1 = rotate_left(s1, rotation_c);
      // operator()();
      bit = _mm_slli_epi64(bit, 1);
    }
  }
  s0 = result0;
  s1 = result1;
}

inline void xoroshiro128plus::long_jump() noexcept {
  // Magic numbers depend on rotation and shift arguments.
  const simd_type jump_mask[] = {_mm_set1_epi64x(0xd2a98b26625eee7bul),
                                 _mm_set1_epi64x(0xdddf9b1090aa7ac1ul)};
  const auto zero = _mm_setzero_si128();
  const auto one = _mm_set1_epi64x(1);
  auto result0 = zero;
  auto result1 = zero;
  for (int i = 0; i < 2; i++) {
    auto bit = one;
    for (size_t b = 0; b < word_size; ++b) {
      const auto mask = _mm_cmpeq_epi64(_mm_and_si128(jump_mask[i], bit), zero);
      result0 = _mm_xor_si128(result0, _mm_andnot_si128(mask, s0));
      result1 = _mm_xor_si128(result1, _mm_andnot_si128(mask, s1));
      s1 = _mm_xor_si128(s0, s1);
      s0 = _mm_xor_si128(s1, _mm_xor_si128(_mm_slli_epi64(s1, shift_b),
                                           rotate_left(s0, rotation_a)));
      s1 = rotate_left(s1, rotation_c);
      bit = _mm_slli_epi64(bit, 1);
    }
  }
  s0 = result0;
  s1 = result1;
}

using xrsr128p = pxart::simd128::xoroshiro128plus;

}  // namespace pxart::simd128