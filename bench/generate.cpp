#include <celero/Celero.h>

#include <PerfEvent.hpp>
#include <array>
#include <boost/random/mersenne_twister.hpp>
#include <iostream>
#include <pxart/msws.hpp>
#include <pxart/mt19937.hpp>
#include <pxart/xoroshiro128plus.hpp>
#include <random>
#include <sstream>

using namespace std;

struct Runner {
  explicit Runner(size_t samples) : n{samples} {
    params.setParam("n", to_string(n));
  }

  template <typename RNG>
  Runner& run_std(const char* name) noexcept {
    RNG rng{rd()};
    for (auto i = 100; i > 0; --i) {
      celero::DoNotOptimizeAway(rng());
    }
    {
      params.setParam("name", name);
      PerfEventBlock e(n, params, true);
      for (auto i = n; i > 0; --i) {
        celero::DoNotOptimizeAway(rng());
      }
    }
    return *this;
  }

  template <typename RNG>
  Runner& run(const char* name) noexcept {
    RNG rng{rd};
    for (auto i = 100; i > 0; --i) {
      celero::DoNotOptimizeAway(rng());
    }
    {
      params.setParam("name", name);
      PerfEventBlock e(n, params, true);
      for (auto i = n; i > 0; --i) {
        celero::DoNotOptimizeAway(rng());
      }
    }
    return *this;
  }

  template <typename RNG>
  Runner& run2(const char* name) noexcept {
    RNG rng1{rd};
    RNG rng2{rd};
    for (auto i = 100; i > 0; i -= 2) {
      celero::DoNotOptimizeAway(rng1());
      celero::DoNotOptimizeAway(rng2());
    }
    {
      params.setParam("name", name);
      PerfEventBlock e(n, params, true);
      for (auto i = n; i > 0; i -= 2) {
        celero::DoNotOptimizeAway(rng1());
        celero::DoNotOptimizeAway(rng2());
      }
    }
    return *this;
  }

  template <typename RNG>
  Runner& run4(const char* name) noexcept {
    RNG rng1{rd};
    RNG rng2{rd};
    RNG rng3{rd};
    RNG rng4{rd};
    for (auto i = 100; i > 0; i -= 4) {
      celero::DoNotOptimizeAway(rng1());
      celero::DoNotOptimizeAway(rng2());
      celero::DoNotOptimizeAway(rng3());
      celero::DoNotOptimizeAway(rng4());
    }
    {
      params.setParam("name", name);
      PerfEventBlock e(n, params, true);
      for (auto i = n; i > 0; i -= 4) {
        celero::DoNotOptimizeAway(rng1());
        celero::DoNotOptimizeAway(rng2());
        celero::DoNotOptimizeAway(rng3());
        celero::DoNotOptimizeAway(rng4());
      }
    }
    return *this;
  }

  size_t n{};
  random_device rd{};
  BenchmarkParameters params{};
};

int main(int argc, char** argv) {
  if (2 != argc) {
    cout << "usage:\n" << argv[0] << " <number of elements>\n";
    return -1;
  }

  stringstream input{argv[1]};
  uint64_t n;
  input >> n;

  Runner bench{n};
  bench  //
      .run_std<std::mt19937>("std::mt19937")
      .run_std<boost::random::mt19937>("boost::mt19937")
      .run<pxart::mt19937>("pxart::mt19937")
      .run<pxart::xrsr128p>("pxart::xrsr128p")
      .run2<pxart::xrsr128p>("pxart::xrsr128p x 2")
      .run4<pxart::xrsr128p>("pxart::xrsr128p x 4")
      .run<pxart::msws>("pxart::msws")
      .run2<pxart::msws>("pxart::msws x 2")
      .run4<pxart::msws>("pxart::msws x 4");
}