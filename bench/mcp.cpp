#include <iostream>
#include <random>
#include <sstream>
//
#include <PerfEvent.hpp>
//
#include <boost/random/mersenne_twister.hpp>
//
#include <pxart/pxart.hpp>

using namespace std;

struct benchmark {
  explicit benchmark(size_t samples) : n{samples} {
    cout << "iteration = " << n << "\n"
         << "\n";
  }

  template <typename RNG, typename Function>
  benchmark& run_function(const char* name, RNG&& rng,
                          Function function) noexcept {
    float pi{};
    {
      params.setParam("                                  name", name);
      PerfEventBlock e(n, params, header);
      pi = function(forward<RNG>(rng), n);
    }
    cout << "pi = " << pi << "\n";

    header = false;
    return *this;
  }
  template <typename RNG, typename Function>
  benchmark& run_function(const char* name, RNG&& rng1, RNG&& rng2,
                          Function function) noexcept {
    float pi{};
    {
      params.setParam("                                  name", name);
      PerfEventBlock e(n, params, header);
      pi = function(forward<RNG>(rng1), forward<RNG>(rng2), n);
    }
    cout << "pi = " << pi << "\n";

    header = false;
    return *this;
  }

  template <typename RNG>
  benchmark& run_std(const char* name, RNG&& rng) noexcept {
    return run_function(name, forward<RNG>(rng), [](RNG&& rng, uint32_t n) {
      return pxart::gallery::base::monte_carlo_pi<float>(forward<RNG>(rng), n);
    });
  }
  template <typename RNG>
  benchmark& run_std(const char* name, RNG&& rng1, RNG&& rng2) noexcept {
    return run_function(name, forward<RNG>(rng1), forward<RNG>(rng2),
                        [](RNG&& rng1, RNG&& rng2, uint32_t n) {
                          return pxart::gallery::base::monte_carlo_pi<float>(
                              forward<RNG>(rng1), forward<RNG>(rng2), n);
                        });
  }
  template <typename RNG>
  benchmark& run_uniform(const char* name, RNG&& rng) noexcept {
    return run_function(name, forward<RNG>(rng), [](RNG&& rng, uint32_t n) {
      return pxart::gallery::monte_carlo_pi<float>(forward<RNG>(rng), n);
    });
  }
  template <typename RNG>
  benchmark& run_uniform(const char* name, RNG&& rng1, RNG&& rng2) noexcept {
    return run_function(name, forward<RNG>(rng1), forward<RNG>(rng2),
                        [](RNG&& rng1, RNG&& rng2, uint32_t n) {
                          return pxart::gallery::monte_carlo_pi<float>(
                              forward<RNG>(rng1), forward<RNG>(rng2), n);
                        });
  }

  template <typename RNG>
  benchmark& run_cache(const char* name, RNG&& rng) noexcept {
    return run_function(name, forward<RNG>(rng), [](RNG&& rng, uint32_t n) {
      return pxart::gallery::cache::monte_carlo_pi<float>(forward<RNG>(rng), n);
    });
  }
  template <typename RNG>
  benchmark& run_cache(const char* name, RNG&& rng1, RNG&& rng2) noexcept {
    return run_function(name, forward<RNG>(rng1), forward<RNG>(rng2),
                        [](RNG&& rng1, RNG&& rng2, uint32_t n) {
                          return pxart::gallery::cache::monte_carlo_pi<float>(
                              forward<RNG>(rng1), forward<RNG>(rng2), n);
                        });
  }

  template <typename RNG>
  benchmark& run_simd256(const char* name, RNG&& rng) noexcept {
    return run_function(name, forward<RNG>(rng), [](RNG&& rng, uint32_t n) {
      return pxart::gallery::simd256::vprng::monte_carlo_pi(forward<RNG>(rng),
                                                            n);
    });
  }
  template <typename RNG>
  benchmark& run_simd256(const char* name, RNG&& rng1, RNG&& rng2) noexcept {
    return run_function(name, forward<RNG>(rng1), forward<RNG>(rng2),
                        [](RNG&& rng1, RNG&& rng2, uint32_t n) {
                          return pxart::gallery::simd256::vprng::monte_carlo_pi(
                              forward<RNG>(rng1), forward<RNG>(rng2), n);
                        });
  }

  template <typename RNG>
  benchmark& run_simd128(const char* name, RNG&& rng) noexcept {
    return run_function(name, forward<RNG>(rng), [](RNG&& rng, uint32_t n) {
      return pxart::gallery::simd128::vprng::monte_carlo_pi(forward<RNG>(rng),
                                                            n);
    });
  }
  template <typename RNG>
  benchmark& run_simd128(const char* name, RNG&& rng1, RNG&& rng2) noexcept {
    return run_function(name, forward<RNG>(rng1), forward<RNG>(rng2),
                        [](RNG&& rng1, RNG&& rng2, uint32_t n) {
                          return pxart::gallery::simd128::vprng::monte_carlo_pi(
                              forward<RNG>(rng1), forward<RNG>(rng2), n);
                        });
  }

  benchmark& separate() noexcept {
    header = true;
    cout << "\n";
    return *this;
  }

  size_t n{};
  BenchmarkParameters params{};
  bool header = true;
};

int main(int argc, char** argv) {
  if (2 != argc) {
    cout << "usage:\n" << argv[0] << " <number of elements>\n";
    return -1;
  }

  stringstream input{argv[1]};
  uint64_t n;
  input >> n;

  random_device rd{};

  benchmark{n}  //
      .run_std("base std::mt19937", std::mt19937{rd()})
      .run_uniform("uniform std::mt19937", std::mt19937{rd()})
      .separate()
      .run_std("base boost::mt19937", boost::random::mt19937{rd()})
      .run_uniform("uniform boost::mt19937", boost::random::mt19937{rd()})
      .separate()
      .run_std("base pxart::mt19937", pxart::mt19937{rd})
      .run_uniform("uniform pxart::mt19937", pxart::mt19937{rd})
      .run_cache("cache pxart::simd256::mt19937", pxart::simd256::mt19937{rd})
      .run_simd256("simd256 pxart::simd256::mt19937",
                   pxart::simd256::mt19937{rd})
      .run_cache("cache pxart::simd128::mt19937", pxart::simd128::mt19937{rd})
      .run_simd128("simd128 pxart::simd128::mt19937",
                   pxart::simd128::mt19937{rd})
      .separate()
      .run_std("base pxart::xrsr128p", pxart::xrsr128p{rd})
      .run_std("2 x base pxart::xrsr128p", pxart::xrsr128p{rd},
               pxart::xrsr128p{rd})
      .run_uniform("uniform pxart::xrsr128p", pxart::xrsr128p{rd})
      .run_uniform("2 x uniform pxart::xrsr128p", pxart::xrsr128p{rd},
                   pxart::xrsr128p{rd})
      .run_cache("cache pxart::simd256::xrsr128p", pxart::simd256::xrsr128p{rd})
      .run_cache("2 x cache pxart::simd256::xrsr128p",
                 pxart::simd256::xrsr128p{rd}, pxart::simd256::xrsr128p{rd})
      .run_simd256("simd256 pxart::simd256::xrsr128p",
                   pxart::simd256::xrsr128p{rd})
      .run_simd256("2 x simd256 pxart::simd256::xrsr128p",
                   pxart::simd256::xrsr128p{rd}, pxart::simd256::xrsr128p{rd})
      .run_cache("cache pxart::simd128::xrsr128p", pxart::simd128::xrsr128p{rd})
      .run_cache("2 x cache pxart::simd128::xrsr128p",
                 pxart::simd128::xrsr128p{rd}, pxart::simd128::xrsr128p{rd})
      .run_simd128("simd128 pxart::simd128::xrsr128p",
                   pxart::simd128::xrsr128p{rd})
      .run_simd128("2 x simd128 pxart::simd128::xrsr128p",
                   pxart::simd128::xrsr128p{rd}, pxart::simd128::xrsr128p{rd})
      .separate()
      .run_std("base pxart::msws", pxart::msws{rd})
      .run_std("2 x base pxart::msws", pxart::msws{rd}, pxart::msws{rd})
      .run_uniform("uniform pxart::msws", pxart::msws{rd})
      .run_uniform("2 x uniform pxart::msws", pxart::msws{rd}, pxart::msws{rd})
      .run_cache("cache pxart::simd256::msws", pxart::simd256::msws{rd})
      .run_cache("2 x cache pxart::simd256::msws", pxart::simd256::msws{rd},
                 pxart::simd256::msws{rd})
      .run_simd256("simd256 pxart::simd256::msws", pxart::simd256::msws{rd})
      .run_simd256("2 x simd256 pxart::simd256::msws", pxart::simd256::msws{rd},
                   pxart::simd256::msws{rd})
      .run_cache("cache pxart::simd128::msws", pxart::simd128::msws{rd})
      .run_cache("2 x cache pxart::simd128::msws", pxart::simd128::msws{rd},
                 pxart::simd128::msws{rd})
      .run_simd128("simd128 pxart::simd128::msws", pxart::simd128::msws{rd})
      .run_simd128("2 x simd128 pxart::simd128::msws", pxart::simd128::msws{rd},
                   pxart::simd128::msws{rd})
      //
      ;
}