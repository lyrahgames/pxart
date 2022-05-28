#pragma once
#include <cmath>
//
#include <pxart/meta.hpp>

namespace pxart {

constexpr float pi = M_PI;
constexpr float two_pi = 2 * M_PI;
constexpr float inv_two_pi = 1 / two_pi;

namespace detail {

template <size_t n>
constexpr float c = 1.0 / (n * (n - 1));

PXART_TEMPLATE(generic::floating_point, real)
constexpr real cos(real x) {
  using namespace std;
  // Make periodic in [-π,π)
  x = real(0.5) + x / (2 * pi<real>);
  x = pi<real> * (2 * (x - floor(x)) - 1);
  //
  x = abs(x);
  x = x - pi / 2;
  //
  const auto x2 = x * x;
  auto factor = -x;
  auto result = factor;
  for (size_t i = 3; i <= 11; i += 2) {
    factor *= -x2 / (i * (i - 1));
    result += factor;
  }
  return result;
}

}  // namespace detail

}  // namespace pxart