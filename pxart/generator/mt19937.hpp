#pragma once
#include <cstdint>
//
#include <pxart/meta.hpp>
#include <pxart/utility.hpp>

namespace pxart {

struct mt19937 {
  using uint_type = uint32_t;
  using result_type = uint_type;
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

  static constexpr uint64_t upper_mask_2 =
      (static_cast<uint64_t>(upper_mask) << 32) |
      static_cast<uint64_t>(upper_mask);
  static constexpr uint64_t lower_mask_2 =
      (static_cast<uint64_t>(lower_mask) << 32) |
      static_cast<uint64_t>(lower_mask);

  struct default_seeder;

  // Seeding Constructor
  // template <typename RNG>
  // requires generic::seeder_for<RNG, mt19937>  //
  PXART_TEMPLATE(generic::type, RNG, (generic::seeder_for<RNG, mt19937>))
  constexpr explicit mt19937(RNG&& rng) {
    generate(std::forward<RNG>(rng), state, state_size);
  }

  // Default Constructor
  constexpr mt19937();

  // Generation
  constexpr auto operator()();

  // Characteristics
  static constexpr auto min() { return uint_type{}; }
  static constexpr auto max() { return ~uint_type{}; }

  // State
  uint_type state[state_size];
  size_t state_index = state_size;
};

struct mt19937::default_seeder {
  // Default and Parameter Constructor
  constexpr explicit default_seeder(uint_type s = default_seed) : x{s} {}

  // Generation
  constexpr auto operator()() {
    const auto result = x;
    x = (x ^ (x >> (word_size - 2)));
    x = (init_multiplier * x + (++c)) & mask;
    return result;
  }

  // Characteristics
  static constexpr uint_type min() { return uint_type{}; }
  static constexpr uint_type max() { return ~uint_type{}; }

  // State
  uint_type x;
  uint_type c{0};
};

// Seeding Constructor
// template <typename RNG>
// requires generic::seeder_for<RNG, mt19937>  //
// PXART_TEMPLATE(generic::type, RNG, (generic::seeder_for<RNG, mt19937>))
// constexpr mt19937::mt19937(RNG&& rng) {
//   generate(std::forward<RNG>(rng), state, state_size);
// }

// Default Constructor
constexpr mt19937::mt19937() : mt19937{default_seeder{}} {}

// Generation
constexpr auto mt19937::operator()() {
  if (state_index >= state_size) {
    const auto transition = [this](size_t k, size_t k1, size_t k2) {
      const auto x = (state[k] & upper_mask) | (state[k1] & lower_mask);
      // state[k] = state[k2] ^ (x >> 1) ^ ((x & 0x01) * xor_mask);
      state[k] = state[k2] ^ (x >> 1) ^ ((state[k1] & 0x01) * xor_mask);
    };

    for (size_t k = 0; k < state_size - shift_size; ++k)
      transition(k, k + 1, k + shift_size);
    for (size_t k = state_size - shift_size; k < state_size - 1; ++k)
      transition(k, k + 1, k + shift_size - state_size);
    transition(state_size - 1, 0, shift_size - 1);

    state_index = 0;
  }

  auto y = state[state_index++];
  y ^= (y >> tempering_u_shift);
  y ^= (y << tempering_s_shift) & tempering_b_mask;
  y ^= (y << tempering_t_shift) & tempering_c_mask;
  y ^= (y >> tempering_l_shift);
  return y;
}

}  // namespace pxart