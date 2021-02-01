#pragma once
#include <cstdint>
//
#include <pxart/meta.hpp>

namespace pxart {

struct minstd_rand {
  // Representation Types
  using uint_type = uint64_t;
  using result_type = uint32_t;

  // Constexpr Parameters
  static constexpr uint_type multiplier = 48271;
  static constexpr uint_type modulus = 2147483647;

  // Default and Parameter Constructors
  constexpr minstd_rand() = default;
  constexpr explicit minstd_rand(uint_type seed) : state{seed} {}

  // Seeding Constructor
  PXART_TEMPLATE(generic::type, G, (generic::seeder_for<G, minstd_rand>))
  // template <typename G, std::enable_if_t<generic::generator<G>, int> = 0>
  constexpr explicit minstd_rand(G&& g) {
    generate(std::forward<G>(g), &state, 1);
  }

  // Generation
  constexpr auto operator()() {
    state = (multiplier * state) % modulus;
    return static_cast<result_type>(state);
  }

  // Specialization of Default Uniform Distribution on [0,1)
  PXART_TEMPLATE(generic::floating_point, R)
  constexpr auto uniform() {
    // The compiler will optimize away the division because it is a constexpr.
    // Manual optimization based on precomputed inverse was slower.
    return R(operator()() - 1) / R(modulus - 1);
  }
  // PXART_TEMPLATE(generic::floating_point, R)
  // static constexpr auto uniform_scale = 1 / R(modulus - 2);
  // constexpr auto uniform() { return R(operator()() - 1) * uniform_scale<R>; }

  // Specialization of Uniform Distribution with Parameters
  PXART_TEMPLATE(generic::floating_point, R)
  constexpr auto uniform(R a, R b) { return uniform<R>() * (b - a) + a; }

  // Characteristics
  static constexpr auto min() { return result_type{1}; }
  static constexpr auto max() { return result_type{modulus - 1}; }

  // State
  uint_type state{1};
};

}  // namespace pxart