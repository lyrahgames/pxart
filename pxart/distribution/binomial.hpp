#pragma once
#include <cassert>
#include <cmath>
#include <type_traits>
#include <vector>
//
#include <pxart/distribution/bernoulli.hpp>
#include <pxart/distribution/uniform.hpp>
#include <pxart/meta.hpp>

namespace pxart {

namespace simulation {

// template <generic::unsigned_integral R, generic::floating_point U>
// struct binomial {
//   using real_type = U;
//   using uint_type = R;

//   binomial(uint_type nn, real_type pp)
//       : count_ones{pp <= 0.5},
//         n{nn},
//         p{count_ones ? pp : (1 - pp)},
//         inv_p{1 / p},
//         inv_q{1 / (1 - p)} {}

//   template <generic::random_bit_generator G>
//   auto operator()(G&& g) const {
//     uint_type result = 0;
//     // auto u = uniform<U>(std::forward<G>(g));
//     for (uint_type i = 0; i < n; ++i) {
//       const auto u = uniform<real_type>(g);
//       result += (u <= p);
//       // Do the coin flip.
//       // const auto bit = (u <= p);
//       // This transformation can be seen as a transition function from a
//       custom
//       // specialized PRNG for the binomial distribution seeded with the
//       // uniformly distributed pseudorandom number from the given generator.
//       // Because the set of possible output values consists only of two
//       values
//       // (0 and 1), this simpler and faster transition can be used to speed
//       up
//       // the simulation process. The underlying floating-point seed value
//       should
//       // provide 23 up to 52 pseudorandom bits. Hence, this custom transition
//       // should possess a period of up to 2^23 or even 2^52 and is therefore
//       // perfectly valid in this scenario when n is smaller than 2^21 or
//       2^50.
//       // u = bit ? (u * inv_p) : ((u - p) * inv_q);
//       // result += bit;
//     }
//     // This may halve the expected runtime because we count the smaller
//     number. return count_ones ? result : (n - result);
//     // return result;
//   }

//  private:
//   bool count_ones;
//   uint_type n;
//   real_type p;
//   real_type inv_p;
//   real_type inv_q;
// };

// template <generic::integral R, generic::floating_point U>
// binomial(R n, U p) -> binomial<std::make_unsigned_t<R>, U>;

// template <generic::unsigned_integral U, generic::floating_point R,
//           generic::random_bit_generator G>
PXART_TEMPLATE(generic::unsigned_integral, U, generic::floating_point, R,
               generic::random_bit_generator, G)
inline auto binomial(G&& g, U n, R p) {
  assert(n > 0);
  assert((0 < p) && (p < 1));

  U result = 0;
  for (U i = 0; i < n; ++i) result += bernoulli<U>(g, p);
  return result;
}

}  // namespace simulation

namespace inversion {

// template <generic::unsigned_integral R, generic::floating_point U>
PXART_TEMPLATE(generic::unsigned_integral, R, generic::floating_point, U)
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

  // template <generic::random_bit_generator G>
  PXART_TEMPLATE(generic::random_bit_generator, G)
  auto operator()(G&& g) const {
    const auto u = uniform<U>(std::forward<G>(g));
    for (uint_type i = 0; i < n; ++i)
      if (u < cdf_table[i]) return i;
    return n;
  }

  auto mean() const { return static_cast<real_type>(n * p); }
  auto var() const { return static_cast<real_type>(n * p * (1 - p)); }
  auto stddev() const { return std::sqrt(var()); }
  auto probability(uint_type k) const {
    assert(k <= n);
    if (k == n) return 1 - cdf_table[n - 1];
    return k ? (cdf_table[k] - cdf_table[k - 1]) : cdf_table[0];
  }
  auto cdf(uint_type k) const {
    if (k >= n) return 1.0f;
    return cdf_table[k];
  }

 private:
  std::vector<real_type> cdf_table;
  uint_type n;
  real_type p;
};

// template <generic::integral R, generic::floating_point U>
PXART_TEMPLATE(generic::integral, R, generic::floating_point, U)
binomial(R n, U p)->binomial<std::make_unsigned_t<R>, U>;

}  // namespace inversion

using inversion::binomial;

}  // namespace pxart