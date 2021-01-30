#pragma once
#include <cmath>
//
#include <pxart/distribution/uniform.hpp>
#include <pxart/meta.hpp>

namespace pxart {

namespace detail {

// Default Exponential Transformation
// template <generic::floating_point R>
PXART_TEMPLATE(generic::floating_point, R)
inline auto exponential(R u) {
  // u1 must not be zero. Map from [0,1) to (0,1].
  u = nextafter(u, R{2.0});
  return -std::log(u);
}

// Exponential Transformation with Parameter
// template <generic::floating_point R>
PXART_TEMPLATE(generic::floating_point, R)
inline auto exponential(R u, R lambda) { return exponential(u) / lambda; }

}  // namespace detail

// Standard Exponential Distribution
// template <generic::floating_point R, generic::random_bit_generator G>
PXART_TEMPLATE(generic::floating_point, R, generic::random_bit_generator, G)
inline auto exponential(G&& g) {
  return detail::exponential(uniform<R>(std::forward<G>(g)));
}

// Exponential Distribution with Parameter
// template <generic::floating_point R, generic::random_bit_generator G>
PXART_TEMPLATE(generic::floating_point, R, generic::random_bit_generator, G)
inline auto exponential(G&& g, R lambda) {
  return detail::exponential(uniform<R>(std::forward<G>(g)), lambda);
}

}  // namespace pxart