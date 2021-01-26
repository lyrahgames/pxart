#pragma once
#include <cassert>
#include <cmath>
#include <string>
//
#include <pxart/distribution/uniform.hpp>
#include <pxart/meta.hpp>

namespace pxart {

namespace detail {

// Default Logistic Transformation
template <generic::floating_point R>
inline auto logistic(R u) {
  // 1-u cannot be zero.
  const auto tmp = std::log(1 - u);
  // u must not be zero. Map from [0,1) to (0,1].
  u = nextafter(u, R{2.0});
  return std::log(u) - tmp;
}

// Logistic Transformation with Parameters
template <generic::floating_point R>
inline auto logistic(R u, R mu, R s) {
  return mu + s * logistic(u);
}

}  // namespace detail

// Default Logistic Distribution
template <generic::floating_point R, generic::random_bit_generator G>
inline auto logistic(G&& g) {
  return detail::logistic(uniform<R>(std::forward<G>(g)));
}

// Logistic Distribution with Parameters
template <generic::floating_point R, generic::random_bit_generator G>
inline auto logistic(G&& g, R mu, R s) {
  return detail::logistic(uniform<R>(std::forward<G>(g)), mu, s);
}

// This structure provides characteristics and properties
// of the logistic distribution.
template <generic::floating_point R>
struct logistic_distribution {
  using real = R;

  explicit logistic_distribution(real mu = 0, real s = 1) : mu{mu}, s{s} {}
  auto mean() const { return mu; }
  auto variance() const { return s * s * real(M_PI) * real(M_PI) / 3; }
  auto sigma() const { return s * real(M_PI) / sqrt(real(3)); }
  auto pdf(real x) const {
    const auto tmp = std::exp(-(x - mu) / s);
    return tmp / (s * (1 + tmp) * (1 + tmp));
  }
  auto pdf_string() const {
    return std::string("exp(-(x-") + std::to_string(mu) + ")/" +
           std::to_string(s) + ")/(" + std::to_string(s) + "*(1+exp(-(x-" +
           std::to_string(mu) + ")/" + std::to_string(s) + "))**2)";
  }
  auto cdf(real x) const { return 1 / (1 + std::exp(-(x - mu) / s)); }

  real mu;
  real s;
};

}  // namespace pxart