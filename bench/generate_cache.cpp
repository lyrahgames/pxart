#include <PerfEvent.hpp>
#include <array>
#include <boost/random/mersenne_twister.hpp>
#include <iostream>
#include <pxart/msws.hpp>
#include <pxart/mt19937.hpp>
#include <pxart/simd128/msws.hpp>
#include <pxart/simd128/mt19937.hpp>
#include <pxart/simd128/xoroshiro128plus.hpp>
#include <pxart/simd256/msws.hpp>
#include <pxart/simd256/mt19937.hpp>
#include <pxart/simd256/xoroshiro128plus.hpp>
#include <pxart/xoroshiro128plus.hpp>
#include <random>
#include <sstream>

using namespace std;

struct benchmark {
  explicit benchmark(size_t samples) : n{samples} {
    cout << "# iteration = " << n << "\n"
         << "# cache size = " << cache_size << " B\n"
         << "# samples = " << n * cache_size / sizeof(uint32_t) << "\n"
         << "\n";
  }

  template <typename result_type, typename RNG>
  benchmark& run(const char* name, RNG&& rng) noexcept {
    constexpr auto cache_count = cache_size / sizeof(result_type);
    array<result_type, cache_count> cache;

    for (int i = 0; i < 100; ++i)
      for (auto& x : cache) x = rng();

    {
      params.setParam("npc",  // numbers per call
                      to_string(sizeof(result_type) / sizeof(uint32_t)));
      params.setParam("#     name", name);
      PerfEventBlock e(n * cache_count, params, header);
      for (int i = 0; i < n; ++i)
        for (auto& x : cache) x = rng();
    }

    // cout << reinterpret_cast<uint32_t&>(
    //             cache[reinterpret_cast<uint32_t&>(cache[0]) % cache_count])
    //      << "\n";
    header = false;
    return *this;
  }

  template <typename RNG>
  benchmark& run(const char* name, RNG&& rng) noexcept {
    return run<decltype(rng())>(name, forward<RNG>(rng));
  }

  template <typename result_type, typename RNG>
  benchmark& run(const char* name, RNG&& rng1, RNG&& rng2) noexcept {
    constexpr auto cache_count = cache_size / sizeof(result_type);
    array<result_type, cache_count> cache;

    for (int i = 0; i < 100; ++i) {
      for (int j = 0; j < cache_count; j += 2) {
        cache[j] = rng1();
        cache[j + 1] = rng2();
      }
    }

    {
      params.setParam("npc",  // numbers per call
                      to_string(sizeof(result_type) / sizeof(uint32_t)));
      params.setParam("#     name", name);
      PerfEventBlock e(n * cache_count, params, header);
      for (int i = 0; i < n; ++i) {
        for (int j = 0; j < cache_count; j += 2) {
          cache[j] = rng1();
          cache[j + 1] = rng2();
        }
      }
    }

    // cout << reinterpret_cast<uint32_t&>(
    //             cache[reinterpret_cast<uint32_t&>(cache[0]) % cache_count])
    //      << "\n";
    header = false;
    return *this;
  }

  template <typename RNG>
  benchmark& run(const char* name, RNG&& rng1, RNG&& rng2) noexcept {
    return run<decltype(rng1())>(name, forward<RNG>(rng1), forward<RNG>(rng2));
  }

  benchmark& separate() noexcept {
    header = true;
    cout << "\n";
    return *this;
  }

  benchmark& heading(const char* text) noexcept {
    cout << text << "\n";
    return *this;
  }

  static constexpr int cache_size = 1 << 14;
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
      .heading("# STL MT19937")
      .run<uint32_t>("scalar", std::mt19937{rd()})
      .separate()
      .heading("# Boost MT19937")
      .run("scalar", boost::random::mt19937{rd()})
      .separate()
      .heading("# MT19937")
      .run("scalar", pxart::mt19937{pxart::mt19937::default_seeder{rd()}})
      .run("AVX", pxart::simd256::mt19937{pxart::mt19937::default_seeder{rd()}})
      .run("SSE", pxart::simd128::mt19937{pxart::mt19937::default_seeder{rd()}})
      .separate()
      .heading("# Xoroshiro128+")
      .run("scalar", pxart::xrsr128p{rd})
      .run("2 x scalar", pxart::xrsr128p{rd}, pxart::xrsr128p{rd})
      .run("AVX", pxart::simd256::xrsr128p{rd})
      .run("2 x AVX", pxart::simd256::xrsr128p{rd},
           pxart::simd256::xrsr128p{rd})
      .run("SSE", pxart::simd128::xrsr128p{rd})
      .run("2 x SSE", pxart::simd128::xrsr128p{rd},
           pxart::simd128::xrsr128p{rd})
      .separate()
      .heading("# MSWS")
      .run("scalar", pxart::msws{rd})
      .run("2 x scalar", pxart::msws{rd}, pxart::msws{rd})
      .run("AVX", pxart::simd256::msws{rd})
      .run("2 x AVX", pxart::simd256::msws{rd}, pxart::simd256::msws{rd})
      .run("SSE", pxart::simd128::msws{rd})
      .run("2 x SSE", pxart::simd128::msws{rd}, pxart::simd128::msws{rd});
}