#pragma once
#include <pxart/distribution/uniform.hpp>
#include <pxart/meta.hpp>

namespace pxart {

namespace detail {

// template <generic::integral U = bool, generic::floating_point R>
PXART_TEMPLATE(generic::integral, U, generic::floating_point, R)
inline auto bernoulli(R u, R p) { return U(u <= p); }

}  // namespace detail

// template <generic::integral U = bool, generic::floating_point R,
//           generic::random_bit_generator G>
PXART_TEMPLATE(generic::integral, U, generic::floating_point, R,
               generic::random_bit_generator, G)
inline auto bernoulli(G&& g, R p) {
  return detail::bernoulli<U>(uniform<R>(std::forward<G>(g)), p);
}

}  // namespace pxart