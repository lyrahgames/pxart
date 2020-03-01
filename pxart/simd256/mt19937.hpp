#pragma once
#include <immintrin.h>

#include <pxart/algorithm.hpp>
#include <pxart/mt19937.hpp>

namespace pxart::simd256 {

struct mt19937 {
  using uint_type = uint32_t;
  using result_type = uint_type;
  using simd_type = __m256i;
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

  uint_type state[state_size + simd_size] __attribute__((aligned(32)));
  int state_index = state_size;
};

template <typename RNG>
inline mt19937::mt19937(RNG&& rng) {
  generate(std::forward<RNG>(rng), state, state + state_size);
}

inline mt19937::mt19937() : mt19937{pxart::mt19937::default_seeder{}} {}

inline mt19937::simd_type mt19937::operator()() noexcept {
  if (state_index >= state_size) {
    const auto transition = [this](int k, int k_shift) constexpr {
      const auto simd_upper_mask = _mm256_set1_epi32(upper_mask);
      const auto simd_lower_mask = _mm256_set1_epi32(lower_mask);
      const auto simd_zero = _mm256_setzero_si256();
      const auto simd_one = _mm256_set1_epi32(1);
      const auto simd_xor_mask = _mm256_set1_epi32(xor_mask);

      // const auto s0 = reinterpret_cast<const simd_type&>(state[k]);
      // const auto s1 = reinterpret_cast<const simd_type&>(state[k + 1]);
      // const auto ss = reinterpret_cast<const simd_type&>(state[k_shift]);
      const auto s0 =
          _mm256_load_si256(reinterpret_cast<const simd_type*>(&state[k]));
      const auto s1 =
          _mm256_loadu_si256(reinterpret_cast<const simd_type*>(&state[k + 1]));
      const auto ss = _mm256_loadu_si256(
          reinterpret_cast<const simd_type*>(&state[k_shift]));

      const auto y = _mm256_or_si256(_mm256_and_si256(s0, simd_upper_mask),
                                     _mm256_and_si256(s1, simd_lower_mask));
      const auto mag01 = _mm256_and_si256(
          simd_xor_mask,
          _mm256_cmpgt_epi32(_mm256_and_si256(y, simd_one), simd_zero));
      const auto tmp2 = _mm256_xor_si256(_mm256_srli_epi32(y, 1), mag01);
      const auto result = _mm256_xor_si256(ss, tmp2);
      return result;
    };

    // const auto transition2 = [this](int k, int k_shift) constexpr {
    //   const auto simd_upper_mask = _mm256_set1_epi32(upper_mask);
    //   const auto simd_lower_mask = _mm256_set1_epi32(lower_mask);
    //   const auto simd_zero = _mm256_setzero_si256();
    //   const auto simd_one = _mm256_set1_epi32(1);
    //   const auto simd_xor_mask = _mm256_set1_epi32(xor_mask);

    //   const auto s00 = reinterpret_cast<const simd_type&>(state[k]);
    //   const auto s01 = reinterpret_cast<const simd_type&>(state[k + 1]);
    //   const auto s0s = reinterpret_cast<const simd_type&>(state[k_shift]);
    //   const auto s10 = reinterpret_cast<const simd_type&>(state[k +
    //   simd_size]); const auto s11 =
    //       reinterpret_cast<const simd_type&>(state[k + 1 + simd_size]);
    //   const auto s1s =
    //       reinterpret_cast<const simd_type&>(state[k_shift + simd_size]);

    //   const auto y0 = _mm256_or_si256(_mm256_and_si256(s00, simd_upper_mask),
    //                                   _mm256_and_si256(s01,
    //                                   simd_lower_mask));
    //   const auto y1 = _mm256_or_si256(_mm256_and_si256(s10, simd_upper_mask),
    //                                   _mm256_and_si256(s11,
    //                                   simd_lower_mask));
    //   const auto mag0 = _mm256_and_si256(
    //       simd_xor_mask,
    //       _mm256_cmpgt_epi32(_mm256_and_si256(y0, simd_one), simd_zero));
    //   const auto mag1 = _mm256_and_si256(
    //       simd_xor_mask,
    //       _mm256_cmpgt_epi32(_mm256_and_si256(y1, simd_one), simd_zero));
    //   const auto result0 = _mm256_xor_si256(
    //       s0s, _mm256_xor_si256(_mm256_srli_epi32(y0, 1), mag0));
    //   const auto result1 = _mm256_xor_si256(
    //       s1s, _mm256_xor_si256(_mm256_srli_epi32(y1, 1), mag1));
    //   return std::pair{result0, result1};
    // };

    const auto first = transition(0, shift_size);
    // reinterpret_cast<simd_type&>(state[0]) = first;
    // reinterpret_cast<simd_type&>(state[state_size]) = first;
    _mm256_store_si256(reinterpret_cast<simd_type*>(&state[0]), first);
    _mm256_store_si256(reinterpret_cast<simd_type*>(&state[state_size]), first);

    int k = simd_size;
    for (; k < state_size - shift_size; k += simd_size) {
      const auto result = transition(k, k + shift_size);
      // reinterpret_cast<simd_type&>(state[k]) = result;
      _mm256_store_si256(reinterpret_cast<simd_type*>(&state[k]), result);
    }
    for (; k < state_size; k += simd_size) {
      const auto result = transition(k, k + shift_size - state_size);
      // reinterpret_cast<simd_type&>(state[k]) = result;
      _mm256_store_si256(reinterpret_cast<simd_type*>(&state[k]), result);
    }
    // for (; k < state_size - shift_size; k += 2 * simd_size) {
    //   const auto result = transition2(k, k + shift_size);
    //   reinterpret_cast<simd_type&>(state[k]) = result.first;
    //   reinterpret_cast<simd_type&>(state[k + simd_size]) = result.second;
    // }
    // for (; k < state_size - simd_size; k += 2 * simd_size) {
    //   const auto result = transition2(k, k + shift_size - state_size);
    //   reinterpret_cast<simd_type&>(state[k]) = result.first;
    //   reinterpret_cast<simd_type&>(state[k + simd_size]) = result.second;
    // }
    // const auto last =
    //     transition(state_size - simd_size, shift_size - simd_size);
    // reinterpret_cast<simd_type&>(state[state_size - simd_size]) = last;

    state_index = 0;
  }

  // auto x = reinterpret_cast<simd_type&>(state[state_index]);
  auto x = _mm256_load_si256(
      reinterpret_cast<const simd_type*>(&state[state_index]));
  state_index += simd_size;
  x = _mm256_xor_si256(x, _mm256_srli_epi32(x, tempering_u_shift));
  x = _mm256_xor_si256(x,
                       _mm256_and_si256(_mm256_slli_epi32(x, tempering_s_shift),
                                        _mm256_set1_epi32(tempering_b_mask)));
  x = _mm256_xor_si256(x,
                       _mm256_and_si256(_mm256_slli_epi32(x, tempering_t_shift),
                                        _mm256_set1_epi32(tempering_c_mask)));
  x = _mm256_xor_si256(x, _mm256_srli_epi32(x, tempering_l_shift));
  return x;
}

}  // namespace pxart::simd256