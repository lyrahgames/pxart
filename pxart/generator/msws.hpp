#pragma once
#include <cstddef>
#include <cstdint>
//
#include <pxart/meta.hpp>
#include <pxart/utility.hpp>

namespace pxart {

struct msws {
  // Representation Types
  using uint_type = uint64_t;
  using result_type = uint32_t;

  // Default and Parameter Constructor
  constexpr msws() = default;
  constexpr explicit msws(uint_type seed) : s{seed | 0x01} {}

  // Seeding Constructor
  PXART_TEMPLATE(generic::type, G, (generic::seeder_for<G, msws>))
  constexpr explicit msws(G&& g) {
    generate(std::forward<G>(g), &s, 1);
    s |= 0x01;
  }

  // Generation
  constexpr auto operator()() {
    x *= x;
    x += (w += s);
    // We have to do an extra static_cast so that MSVC is not warning.
    return static_cast<result_type>(x = ((x >> 32) | (x << 32)));
  }

  // Characteristics
  static constexpr auto min() { return result_type{}; }
  static constexpr auto max() { return ~result_type{}; }

  // State
  uint_type s = 0xb5ad4eceda1ce2a9;
  uint_type x = 0;
  uint_type w = 0;
};

}  // namespace pxart