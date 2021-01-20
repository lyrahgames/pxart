#pragma once
#include <cassert>
#include <cmath>
#include <type_traits>
#include <vector>
//
#include <pxart/distribution/uniform.hpp>
#include <pxart/meta.hpp>

namespace pxart {

namespace detail {}

template <generic::unsigned_integral R, generic::floating_point U>
struct binomial {
  using real_type = U;
  using uint_type = R;

  binomial(uint_type n, real_type p) : cdf_table(n, 1), n{n}, p{p} {
    assert(n > 0);
    assert((0 <= p) && (p <= 1));

    const auto q = real_type{1} - p;

    // First, compute probability for every event.
    // Compute binomial coefficients.
    real_type coeff = n;
    for (uint_type i = 1; i <= n / 2; ++i) {
      cdf_table[i] = coeff;
      cdf_table[n - i] = coeff;
      coeff *= real_type(n - i) / real_type(i + 1);
    }
    // Compute probability powers.
    real_type pk = p;
    real_type qk = q;
    for (uint_type i = 1; i < n; ++i) {
      cdf_table[i] *= pk;
      cdf_table[n - i] *= qk;
      pk *= p;
      qk *= q;
    }
    cdf_table[0] *= qk;
    // Compute cumulative distribution function by summing up.
    for (uint_type i = 1; i < n; ++i) cdf_table[i] += cdf_table[i - 1];
  }

  template <generic::random_bit_generator G>
  auto operator()(G&& g) const {
    const auto u = uniform<U>(std::forward<G>(g));
    for (uint_type i = 0; i < n; ++i)
      if (u < cdf_table[i]) return i;
    return n;
  }

 private:
  std::vector<real_type> cdf_table;
  uint_type n;
  real_type p;
};

template <generic::integral R, generic::floating_point U>
binomial(R n, U p) -> binomial<std::make_unsigned_t<R>, U>;

}  // namespace pxart