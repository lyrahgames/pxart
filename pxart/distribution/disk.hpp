#pragma once
#include <cmath>
//
#include <pxart/distribution/uniform.hpp>
#include <pxart/meta.hpp>

namespace pxart {

// template <generic::floating_point R, generic::random_bit_generator G>
PXART_TEMPLATE(generic::floating_point, R, generic::random_bit_generator, G)
inline auto disk(G&& g) {
  while (true) {
    const auto x = uniform<R>(g, -1, 1);
    const auto y = uniform<R>(g, -1, 1);
    if (x * x + y * y < R{1}) return std::pair{x, y};
  }
}

}  // namespace pxart