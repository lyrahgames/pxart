#pragma once

#include <cstdint>

namespace rng {

struct middle_square_weyl_engine {
  using result_type = uint32_t;

  constexpr result_type operator()() noexcept {
    state *= state;
    weyl += seed;
    state += weyl;
    return state = (state >> 32) | (state << 32);
  }
  static constexpr result_type min() noexcept { return 0; }
  static constexpr result_type max() noexcept { return 0xffffffff; }

  uint64_t seed = 0xb5ad4eceda1ce2a9;
  uint64_t state = 0;
  uint64_t weyl = 0;
};

}  // namespace rng