#pragma once
#include <cstddef>
#include <cstdint>
#include <pxart/algorithm.hpp>

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

  template <typename RNG>
  constexpr explicit mt19937(RNG&& rng);
  constexpr mt19937();
  mt19937(const mt19937&) = default;
  mt19937& operator=(const mt19937&) = default;
  mt19937(mt19937&&) = default;
  mt19937& operator=(mt19937&&) = default;

  constexpr result_type operator()() noexcept;
  constexpr result_type min() noexcept { return uint_type{}; }
  constexpr result_type max() noexcept { return (~uint_type{}) & mask; }

  uint_type state[state_size];
  // uint_type state[state_size] __attribute__((aligned(64)));
  // uint_type state[state_size + 2] __attribute__((aligned(64)));
  int state_index = state_size;
};

struct mt19937::default_seeder {
  constexpr explicit default_seeder(uint_type s = default_seed) : x{s & mask} {}
  constexpr uint_type operator()() noexcept;
  constexpr uint_type min() noexcept { return uint_type{}; }
  constexpr uint_type max() noexcept { return (~uint_type{}) & mask; }
  uint_type x;
  uint_type c{0};
};

constexpr mt19937::uint_type mt19937::default_seeder::operator()() noexcept {
  const auto result = x;
  x = (x ^ (x >> (word_size - 2)));
  x = (init_multiplier * x + (++c)) & mask;
  return result;
}

template <typename RNG>
constexpr mt19937::mt19937(RNG&& rng) {
  generate(std::forward<RNG>(rng), state, state + state_size);
}

constexpr mt19937::mt19937() : mt19937{default_seeder{}} {}

constexpr mt19937::result_type mt19937::operator()() noexcept {
  if (state_index >= state_size) {
    const auto transition = [this](int k, int k1, int k2) constexpr {
      const auto x = (state[k] & upper_mask) | (state[k1] & lower_mask);
      state[k] = state[k2] ^ (x >> 1) ^ ((x & 0x01) ? xor_mask : 0);
    };

    for (int k = 0; k < state_size - shift_size; ++k)
      transition(k, k + 1, k + shift_size);
    for (int k = state_size - shift_size; k < state_size - 1; ++k)
      transition(k, k + 1, k + shift_size - state_size);
    transition(state_size - 1, 0, shift_size - 1);

    // const auto transition = [this](int k, int k1, int k2) constexpr {
    //   const auto x =
    //       (*reinterpret_cast<const uint64_t*>(&state[k]) & upper_mask_2) |
    //       (*reinterpret_cast<const uint64_t*>(&state[k1]) & lower_mask_2);
    //   return *reinterpret_cast<uint64_t*>(&state[k]) =
    //              *reinterpret_cast<const uint64_t*>(&state[k2]) ^
    //              ((x & 0xfffffffefffffffful) >> 1) ^
    //              ((x & 0x0000000000000001ul) ?
    //              static_cast<uint64_t>(xor_mask)
    //                                          : 0ul) ^
    //              ((x & 0x0000000100000000ul)
    //                   ? (static_cast<uint64_t>(xor_mask) << 32)
    //                   : (0ul));
    // };

    // const auto first = transition(0, 1, shift_size);
    // *reinterpret_cast<uint64_t*>(&state[state_size]) = first;
    // int k = 2;
    // for (; k < state_size - shift_size; k += 2)
    //   transition(k, k + 1, k + shift_size);
    // for (; k < state_size; k += 2)
    //   transition(k, k + 1, k + shift_size - state_size);

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