#pragma once

#include <cstdint>

namespace rng {

struct middle_square_engine {
  using result_type = uint32_t;

  constexpr result_type operator()() noexcept {
    state *= state;
    return state = (state >> 16) & 0xffffffff;
  }
  static constexpr result_type min() noexcept { return 0; }
  static constexpr result_type max() noexcept { return 0xffffffff; }

  uint64_t state = 0xfedcb2ed;
};

}  // namespace rng