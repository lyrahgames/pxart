#pragma once
#include <cmath>
//
#include <pxart/distribution/uniform.hpp>
#include <pxart/meta.hpp>

namespace pxart {

namespace box_muller {

namespace detail {

// Standard Normal Transformation
template <generic::floating_point R>
inline auto normal(R u1, R u2) {
  using namespace std;
  // u1 must not be zero.
  u1 = nextafter(u1, R{2.0});
  constexpr R two_pi = 2.0 * M_PI;
  const auto r = sqrt(R{-2.0} * log(u1));
  const auto z1 = r * cos(two_pi * u2);
  const auto z2 = r * sin(two_pi * u2);
  return std::pair<R, R>{z1, z2};
}

// Normal Transformation
template <generic::floating_point R>
inline auto normal(R u1, R u2, R mu, R sigma) {
  using namespace std;
  // u1 must not be zero.
  u1 = nextafter(u1, R{2.0});
  constexpr R two_pi = 2.0 * M_PI;
  const auto r = sigma * sqrt(R{-2.0} * log(u1));
  const auto z1 = r * cos(two_pi * u2) + mu;
  const auto z2 = r * sin(two_pi * u2) + mu;
  return std::pair<R, R>{z1, z2};
}

}  // namespace detail

// Standard Normal Distribution
template <generic::floating_point R, generic::random_bit_generator G>
inline auto normal(G&& g) {
  const auto u1 = uniform<R>(g);
  const auto u2 = uniform<R>(std::forward<G>(g));
  return detail::normal(u1, u2);
}

// Normal Distribution
template <generic::floating_point R, generic::random_bit_generator G>
inline auto normal(G&& g, R mu, R sigma) {
  const auto u1 = uniform<R>(g);
  const auto u2 = uniform<R>(std::forward<G>(g));
  return detail::normal(u1, u2, mu, sigma);
}

}  // namespace box_muller

namespace irwin_hall {

// Approximation to Standard Normal Distribution
template <generic::floating_point R, generic::random_bit_generator G>
inline auto normal(G&& g) {
  const auto u01 = uniform<R>(g);
  const auto u02 = uniform<R>(g);
  const auto u03 = uniform<R>(g);
  const auto u04 = uniform<R>(g);
  const auto u05 = uniform<R>(g);
  const auto u06 = uniform<R>(g);
  const auto u07 = uniform<R>(g);
  const auto u08 = uniform<R>(g);
  const auto u09 = uniform<R>(g);
  const auto u10 = uniform<R>(g);
  const auto u11 = uniform<R>(g);
  const auto u12 = uniform<R>(std::forward<G>(g));
  return u01 + u02 + u03 + u04 + u05 + u06 + u07 + u08 + u09 + u10 + u11 + u12 -
         R{6.0};
}

// Approximation to Normal Distribution
template <generic::floating_point R, generic::random_bit_generator G>
inline auto normal(G&& g, R mu, R sigma) {
  return sigma * normal(std::forward<G>(g)) + mu;
}

}  // namespace irwin_hall

using namespace box_muller;

}  // namespace pxart