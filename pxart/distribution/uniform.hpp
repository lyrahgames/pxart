#pragma once
#include <pxart/meta.hpp>
#include <pxart/utility.hpp>

namespace pxart {

namespace detail {

// 32-bit Input Uniform Transformation
// template <generic::floating_point R>
PXART_TEMPLATE(generic::floating_point, R)
inline R uniform(uint32_t);
template <>
inline float uniform<float>(uint32_t x) {
  const auto tmp = ((x >> 9) | 0x3f800000);
  return pun_cast<float>(tmp) - 1.0f;
}
template <>
inline double uniform<double>(uint32_t x) {
  const auto tmp = ((static_cast<uint64_t>(x) << 20) | 0x3ff0000000000000ull);
  return pun_cast<double>(tmp) - 1.0;
}

// 64-bit Input Uniform Transformation
// template <generic::floating_point R>
PXART_TEMPLATE(generic::floating_point, R)
inline R uniform(uint64_t);
template <>
inline float uniform<float>(uint64_t x) {
  const auto tmp = (static_cast<uint32_t>(x >> 41) | 0x3f800000);
  return pun_cast<float>(tmp) - 1.0f;
}
template <>
inline double uniform<double>(uint64_t x) {
  const auto tmp = ((x >> 12) | 0x3ff0000000000000ULL);
  return pun_cast<double>(tmp) - 1.0;
}

// Uniform Transformation with Bounds
// template <generic::floating_point R, generic::scalar_bit_packet U>
PXART_TEMPLATE(generic::floating_point, R, generic::scalar_bit_packet, U)
inline R uniform(U x, R a, R b) {  //
  return (b - a) * uniform<R>(x) + a;
}

}  // namespace detail

// Default Uniform Distribution
// template <generic::floating_point R, generic::random_bit_generator G>
PXART_TEMPLATE(generic::floating_point, R, generic::random_bit_generator, G,
               (!generic::has_uniform_01<G, R>))
inline R uniform(G&& g) {  //
  return detail::uniform<R>(std::forward<G>(g)());
}
// template <generic::floating_point R, generic::random_bit_generator G>
// requires generic::has_uniform_01<G, R>  //
PXART_TEMPLATE(generic::floating_point, R, generic::random_bit_generator, G,
               (generic::has_uniform_01<G, R>))
inline R uniform(G&& g) {  //
  return std::forward<G>(g).template uniform<R>();
}

// Uniform Distribution with Bounds
// template <generic::floating_point R, generic::random_bit_generator G>
PXART_TEMPLATE(generic::floating_point, R, generic::random_bit_generator, G,
               (!generic::has_uniform<G, R>))
inline R uniform(G&& g, R a, R b) {  //
  return detail::uniform(std::forward<G>(g)(), a, b);
}
// template <generic::floating_point R, generic::random_bit_generator G>
// requires generic::has_uniform<G, R>  //
PXART_TEMPLATE(generic::floating_point, R, generic::random_bit_generator, G,
               (generic::has_uniform<G, R>))
inline R uniform(G&& g, R a, R b) {  //
  return std::forward<G>(g).uniform(a, b);
}

}  // namespace pxart