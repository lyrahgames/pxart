#pragma once

namespace pxart {

template <typename Real>
struct uniform_real_distribution {
  using real_type = Real;
  using result_type = real_type;
  using param_type = real_type;

  real_type min() const noexcept { return a; }
  real_type max() const noexcept { return b; }

  template <typename RNG>
  real_type operator()(RNG& rng) const noexcept {
    return (static_cast<real_type>(rng()) - static_cast<real_type>(rng.min())) /
               (static_cast<real_type>(rng.max()) -
                static_cast<real_type>(rng.min()) + 1) *
               (b - a) +
           a;
  }

  real_type a, b;
};

}  // namespace pxart