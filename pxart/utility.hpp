#pragma once
#include <cstring>
//
#include <pxart/meta.hpp>

namespace pxart {

// template <generic::random_bit_generator G, std::unsigned_integral U>
PXART_TEMPLATE(generic::random_bit_generator, G, generic::unsigned_integral, U)
inline void generate(G&& g, U* data, size_t n) {
  using uint_type = meta::result<G>;
  if constexpr (generic::equal<uint_type, U>) {
    for (size_t i = 0; i < n; ++i) data[i] = g();
  } else {
    const size_t size = 1 + (n * sizeof(U) - 1) / sizeof(uint_type);
    uint_type buffer[size];
    for (size_t i = 0; i < size; ++i) buffer[i] = g();
    std::memcpy(data, buffer, n * sizeof(U));
  }
}

// template <typename T, typename U>
PXART_TEMPLATE(generic::type, T, generic::type, U)
inline T pun_cast(U x) {
  static_assert(sizeof(T) == sizeof(U));
  T t{};
  std::memcpy(&t, &x, sizeof(x));
  return t;
}

}  // namespace pxart