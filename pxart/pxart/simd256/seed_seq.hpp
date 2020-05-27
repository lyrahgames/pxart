#pragma once
#ifndef __AVX2__
#else
#define PXART_SUPPORT_SIMD256_SEED_SEQ

#include <immintrin.h>

#include <array>
#include <pxart/seed_seq.hpp>
#include <pxart/utilities/pun_cast.hpp>

namespace pxart::simd256 {

struct seed_seq_8x32 {
  using uint_type = uint32_t;
  using result_type = uint_type;
  static constexpr size_t state_size = 8;
  using simd_type = __m256i;
  static constexpr size_t simd_size = sizeof(simd_type) / sizeof(result_type);

  static constexpr uint_type init_a = 0x43b0d7e5;
  static constexpr uint_type mult_a = 0x931e8875;
  static constexpr uint_type init_b = 0x8b51f9dd;
  static constexpr uint_type mult_b = 0x58f38ded;
  static constexpr uint_type mix_multiplier_l = 0xca01f9dd;
  static constexpr uint_type mix_multiplier_r = 0x4973f715;
  static constexpr size_t xor_shift = sizeof(uint_type) * 8 / 2;

  template <uint32_t N>
  struct hasher {
    static constexpr uint32_t multiplier = N;
    static constexpr uint32_t multiplier_2 = multiplier * multiplier;
    static constexpr uint32_t multiplier_3 = multiplier_2 * multiplier;
    static constexpr uint32_t multiplier_4 = multiplier_3 * multiplier;
    static constexpr uint32_t multiplier_5 = multiplier_4 * multiplier;
    static constexpr uint32_t multiplier_6 = multiplier_5 * multiplier;
    static constexpr uint32_t multiplier_7 = multiplier_6 * multiplier;

    hasher(uint32_t init)
        : state{_mm256_mullo_epi32(
              _mm256_set1_epi32(init),
              _mm256_set_epi32(multiplier_7, multiplier_6, multiplier_5,
                               multiplier_4, multiplier_3, multiplier_2,
                               multiplier, 1))} {}

    __m256i operator()(__m256i x) noexcept {
      x = _mm256_xor_si256(x, state);
      state = _mm256_mullo_epi32(state, _mm256_set1_epi32(multiplier));
      x = _mm256_mullo_epi32(x, state);
      state = _mm256_mullo_epi32(state, _mm256_set1_epi32(multiplier_7));
      x = _mm256_xor_si256(x, _mm256_srli_epi32(x, 16));
      return x;
    }

    __m256i state;
  };

  static simd_type mix(simd_type x, simd_type y) noexcept;

  template <typename InputIt>
  seed_seq_8x32(InputIt begin, InputIt end) noexcept;
  template <typename U>
  seed_seq_8x32(std::initializer_list<U> init) noexcept
      : seed_seq_8x32(begin(init), end(init)) {}
  seed_seq_8x32() noexcept : seed_seq_8x32{1, 2, 3, 4} {}
  seed_seq_8x32(const seed_seq_8x32&) = default;
  seed_seq_8x32& operator=(const seed_seq_8x32&) = default;
  seed_seq_8x32(seed_seq_8x32&&) = default;
  seed_seq_8x32& operator=(seed_seq_8x32&&) = default;

  simd_type operator()() noexcept;

  simd_type data;
  hasher<mult_b> hash{init_b};
};

inline auto seed_seq_8x32::mix(simd_type x, simd_type y) noexcept -> simd_type {
  auto result = _mm256_sub_epi32(
      _mm256_mullo_epi32(x, _mm256_set1_epi32(mix_multiplier_l)),
      _mm256_mullo_epi32(y, _mm256_set1_epi32(mix_multiplier_r)));
  result = _mm256_xor_si256(result, _mm256_srli_epi32(result, 16));
  return result;
}

template <typename InputIt>
seed_seq_8x32::seed_seq_8x32(InputIt first, InputIt last) noexcept {
  // For now, we do the entropy mixing in scalar mode.

  pxart::seed_seq_8x32::hasher<mult_a> hash{init_a};
  std::array<uint_type, state_size> tmp{};

  // Initialize internal state by hash functions.
  size_t i = 0;
  InputIt it = first;
  for (; (i < state_size) && (it != last); ++i, ++it)  //
    tmp[i] = hash(*it);
  for (; i < state_size; ++i)  //
    tmp[i] = hash(0);

  // Mix entropy of input.
  for (size_t p = 0; p < state_size; ++p) {
    for (size_t q = 0; q < p; ++q)  //
      tmp[q] = pxart::seed_seq_8x32::mix(tmp[q], hash(tmp[p]));
    for (size_t q = p + 1; q < state_size; ++q)
      tmp[q] = pxart::seed_seq_8x32::mix(tmp[q], hash(tmp[p]));
  }

  // Mix extra entropy given in input into state.
  for (; it != last; ++it)
    for (auto& x : tmp) x = pxart::seed_seq_8x32::mix(x, hash(*it));

  data = pxart::pun_cast<simd_type>(tmp);
}

inline auto seed_seq_8x32::operator()() noexcept -> simd_type {
  return hash(data);
}

}  // namespace pxart::simd256

#endif  // __AVX2__