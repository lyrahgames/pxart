#pragma once
#include <cstdint>

namespace pxart {

struct pcg32 {
  using uint_type = uint64_t;
  using result_type = uint32_t;

  static constexpr uint_type multiplier = 6364136223846793005ull;

  constexpr pcg32() {
    state += inc;
    state = multiplier * state + inc;
  }

  constexpr pcg32(uint_type x, uint_type y) : state{x}, inc{y} {}
  template <typename RNG>
  constexpr explicit pcg32(RNG&& rng)
      : state{(static_cast<uint_type>(rng()) << 32) |
              static_cast<uint_type>(rng())},
        inc{(static_cast<uint_type>(rng()) << 32) |
            static_cast<uint_type>(rng())} {}

  constexpr result_type operator()() {
    const auto old_state = state;
    state = multiplier * old_state + inc;
    result_type xorshifted = ((old_state >> 18u) ^ old_state) >> 27u;
    result_type rot = old_state >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
  }

  uint_type state{0xcafef00dd15ea5e5ULL};
  uint_type inc{1442695040888963407ULL};
};

}  // namespace pxart