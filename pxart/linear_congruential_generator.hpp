#pragma once

#include <cstdint>

namespace rng {

template <typename UIntType, UIntType A, UIntType C, UIntType M>
struct linear_congruential_generator {
  using uint_type = UIntType;
  using result_type = uint_type;

  static constexpr uint_type multiplier = A;
  static constexpr uint_type increment = C;
  static constexpr uint_type modulus = M;
  static constexpr uint_type default_seed = 1;

  static constexpr result_type min() noexcept {
    return (increment == 0) ? (1) : (0);
  }
  static constexpr result_type max() noexcept { return modulus - 1; }

  constexpr linear_congruential_generator& seed(
      uint_type value = default_seed) noexcept {
    state = value;
    return *this;
  }

  constexpr result_type operator()() noexcept {
    return state = (multiplier * state + increment) % modulus;
  }

  uint_type state{default_seed};
};

using randu = linear_congruential_generator<uint_fast32_t, 65539, 0, 1u << 31>;
using minstd_rand =
    linear_congruential_generator<uint_fast32_t, 48271, 0, (1u << 31) - 1>;

}  // namespace rng