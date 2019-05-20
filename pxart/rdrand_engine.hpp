#pragma once

#include <cstdint>

namespace rng {

struct rdrand_engine {
  using result_type = uint32_t;

  result_type operator()() noexcept {
    result_type result{};
    while (!(__builtin_ia32_rdrand32_step(&result)))
      ;
    return result;
  }

  static constexpr result_type min() noexcept { return 0; }
  static constexpr result_type max() noexcept { return 0xffffffff; }
};

}  // namespace rng