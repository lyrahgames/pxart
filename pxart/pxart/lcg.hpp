#pragma once
#include <cstdint>

namespace pxart {

template <typename Result_type, typename Uint_type, Uint_type a, Uint_type c,
          Uint_type m>
struct lcg {
  using uint_type = Uint_type;
  using result_type = Result_type;

  constexpr lcg() = default;
  constexpr explicit lcg(uint_type seed) noexcept : state{seed} {}
  // template <typename RNG>
  // constexpr explicit lcg(RNG&& rng) noexcept
  //     : state{std::forward<RNG>(rng)()} {}

  constexpr result_type operator()() noexcept {
    state = (a * state + c) % m;
    return state;
  }

  uint_type state{1};
};

using minstd_rand = lcg<uint32_t, uint64_t, 48271, 0, 2147483647>;

}  // namespace pxart