#pragma once
#include <cmath>
//
#include <pxart/distribution/uniform.hpp>
#include <pxart/meta.hpp>

namespace pxart {

namespace detail {

// Default Cauchy Transformation
template <generic::floating_point R>
inline auto cauchy(R u) {
  return std::tan(R{M_PI} * (u - R{0.5}));
}

// Cauchy Transformation with Parameters
template <generic::floating_point R>
inline auto cauchy(R u, R location, R hwhm) {
  return hwhm * cauchy(u) + location;
}

}  // namespace detail

// Standard Cauchy Distribution
template <generic::floating_point R, generic::random_bit_generator G>
inline auto cauchy(G&& g) {
  return detail::cauchy(uniform<R>(std::forward<G>(g)));
}

// Cauchy Distribution with Parameters
template <generic::floating_point R, generic::random_bit_generator G>
inline auto cauchy(G&& g, R location, R hwhm) {
  return detail::cauchy(uniform<R>(std::forward<G>(g)), location, hwhm);
}

}  // namespace pxart