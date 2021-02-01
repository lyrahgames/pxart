#pragma once
#include <cstdint>
//
#include <pxart/meta.hpp>

namespace pxart {

struct minstd_rand {
  static constexpr uint64_t multiplier = 48271;
  static constexpr uint64_t modulus = 2147483647;
  // PXART_TEMPLATE(generic::floating_point, R)
  // static constexpr auto uniform_scale = 1 / R(modulus - 2);

  constexpr auto operator()() {
    state = (multiplier * state) % modulus;
    return static_cast<uint32_t>(state);
  }

  PXART_TEMPLATE(generic::floating_point, R)
  constexpr auto uniform() { return R(operator()() - 1) / (modulus - 2); }
  // constexpr auto uniform() { return R(operator()() - 1) * uniform_scale<R>; }

  uint64_t state{1};
};

}  // namespace pxart