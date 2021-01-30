#pragma once
#include <cassert>
#include <cmath>
//
#include <pxart/distribution/uniform.hpp>
#include <pxart/meta.hpp>

namespace pxart {

namespace knuth {

// template <generic::unsigned_integral U, generic::floating_point R,
//           generic::random_bit_generator G>
PXART_TEMPLATE(generic::unsigned_integral, U, generic::floating_point, R,
               generic::random_bit_generator, G)
inline auto poisson(G&& g, R lambda) {
  assert(lambda > 0);

  const auto bound = std::exp(-lambda);
  U result = 0;
  auto u = uniform<R>(g);
  while (u >= bound) {
    ++result;
    u *= uniform<R>(g);
  }
  return result;
}

}  // namespace knuth

// Use the method by Knuth as default method.
using knuth::poisson;

// This structure provides characteristics and properties
// of the Poisson distribution.
// template <generic::floating_point R>
PXART_TEMPLATE(generic::floating_point, R)
struct poisson_distribution {
  using real = R;

  explicit poisson_distribution(real lambda) : lambda{lambda} {
    assert(lambda > 0);
  }
  auto mean() const { return lambda; }
  auto variance() const { return lambda; }
  auto sigma() const { return std::sqrt(lambda); }
  auto probability(size_t k) const {
    using namespace std;
    return exp(k * log(lambda) - lambda - lgamma(real(k + 1)));
  }

  real lambda;
};

}  // namespace pxart