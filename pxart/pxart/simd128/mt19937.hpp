#pragma once
#ifndef __SSE2__
#else
#define PXART_SUPPORT_SIMD128_MT19937

#include <emmintrin.h>

#include <pxart/algorithm.hpp>
#include <pxart/mt19937.hpp>

namespace pxart::simd128 {

struct mt19937 {
  using uint_type = uint32_t;
  using result_type = uint_type;
  using simd_type = __m128i;
  static constexpr size_t simd_size = sizeof(simd_type) / sizeof(result_type);

  static constexpr size_t word_size = 32;
  static constexpr size_t state_size = 624;
  static constexpr size_t shift_size = 397;
  static constexpr size_t mask_bits = 31;
  static constexpr uint_type xor_mask = 0x9908b0dfu;
  static constexpr uint_type tempering_b_mask = 0x9d2c5680u;
  static constexpr uint_type tempering_c_mask = 0xefc60000u;
  static constexpr size_t tempering_u_shift = 11;
  static constexpr size_t tempering_s_shift = 7;
  static constexpr size_t tempering_t_shift = 15;
  static constexpr size_t tempering_l_shift = 18;
  static constexpr uint_type default_seed = 5489u;
  static constexpr uint_type init_multiplier = 1812433253u;

  static constexpr uint_type mask = (~uint_type{}) >>
                                    (sizeof(uint_type) * 8 - word_size);
  static constexpr uint_type upper_mask = ((~uint_type{}) << mask_bits) & mask;
  static constexpr uint_type lower_mask = (~upper_mask) & mask;

  template <typename RNG>
  explicit mt19937(RNG&& rng);
  mt19937();
  mt19937(const mt19937&) = default;
  mt19937& operator=(const mt19937&) = default;
  mt19937(mt19937&&) = default;
  mt19937& operator=(mt19937&&) = default;

  simd_type operator()() noexcept;
  constexpr result_type min() noexcept { return uint_type{}; }
  constexpr result_type max() noexcept { return (~uint_type{}) & mask; }

  // We may have to take care of 16-byte alignment for older systems.
  // As a compiler-independent approach, we have chosen to define the state as
  // an array of vector registers. Hence, the state data is automatically
  // aligned by 16 byte.
  simd_type state_data[1 + state_size / simd_size];
  // uint_type* state = reinterpret_cast<uint_type*>(&state_data[0]);
  // uint_type state[state_size + simd_size];
  size_t state_index = state_size;
};

template <typename RNG>
inline mt19937::mt19937(RNG&& rng) {
  auto state = reinterpret_cast<uint_type*>(&state_data[0]);
  generate(std::forward<RNG>(rng), state, state + state_size);
}

inline mt19937::mt19937() : mt19937{pxart::mt19937::default_seeder{}} {}

inline mt19937::simd_type mt19937::operator()() noexcept {
  auto state = reinterpret_cast<uint_type*>(&state_data[0]);

  if (state_index >= state_size) {
    const auto transition = [state](size_t k, size_t k_shift) {
      const auto simd_upper_mask = _mm_set1_epi32(upper_mask);
      const auto simd_lower_mask = _mm_set1_epi32(lower_mask);
      const auto simd_zero = _mm_setzero_si128();
      const auto simd_one = _mm_set1_epi32(1);
      const auto simd_xor_mask = _mm_set1_epi32(xor_mask);

      const auto s0 =
          _mm_load_si128(reinterpret_cast<const simd_type*>(&state[k]));
      const auto s1 =
          _mm_loadu_si128(reinterpret_cast<const simd_type*>(&state[k + 1]));
      const auto ss =
          _mm_loadu_si128(reinterpret_cast<const simd_type*>(&state[k_shift]));

      const auto y = _mm_or_si128(_mm_and_si128(s0, simd_upper_mask),
                                  _mm_and_si128(s1, simd_lower_mask));
      const auto mag01 =
          _mm_and_si128(simd_xor_mask,
                        _mm_cmpgt_epi32(_mm_and_si128(y, simd_one), simd_zero));
      const auto tmp2 = _mm_xor_si128(_mm_srli_epi32(y, 1), mag01);
      const auto result = _mm_xor_si128(ss, tmp2);
      return result;
    };

    const auto first = transition(0, shift_size);
    _mm_store_si128(reinterpret_cast<simd_type*>(&state[0]), first);
    _mm_store_si128(reinterpret_cast<simd_type*>(&state[state_size]), first);

    size_t k = simd_size;
    for (; k < state_size - shift_size; k += simd_size) {
      const auto result = transition(k, k + shift_size);
      _mm_store_si128(reinterpret_cast<simd_type*>(&state[k]), result);
    }
    for (; k < state_size; k += simd_size) {
      const auto result = transition(k, k + shift_size - state_size);
      _mm_store_si128(reinterpret_cast<simd_type*>(&state[k]), result);
    }

    state_index = 0;
  }

  auto x = reinterpret_cast<simd_type&>(state[state_index]);
  state_index += simd_size;
  x = _mm_xor_si128(x, _mm_srli_epi32(x, tempering_u_shift));
  x = _mm_xor_si128(x, _mm_and_si128(_mm_slli_epi32(x, tempering_s_shift),
                                     _mm_set1_epi32(tempering_b_mask)));
  x = _mm_xor_si128(x, _mm_and_si128(_mm_slli_epi32(x, tempering_t_shift),
                                     _mm_set1_epi32(tempering_c_mask)));
  x = _mm_xor_si128(x, _mm_srli_epi32(x, tempering_l_shift));
  return x;
}

}  // namespace pxart::simd128

#endif