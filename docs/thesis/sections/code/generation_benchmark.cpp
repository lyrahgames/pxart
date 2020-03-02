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
    cout << "iteration = " << n << "\n"
         << "cache size = " << cache_size << " B\n"
         << "samples = " << n * cache_size / sizeof(uint32_t) << "\n"
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
      params.setParam("name", name);
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
      params.setParam("name", name);
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
      .run<uint32_t>("std   mt", std::mt19937{rd()})
      .run("boost mt", boost::random::mt19937{rd()})
      .run("pxart mt", pxart::mt19937{pxart::mt19937::default_seeder{rd()}})
      .run("px mt256",
           pxart::simd256::mt19937{pxart::mt19937::default_seeder{rd()}})
      .run("px mt128",
           pxart::simd128::mt19937{pxart::mt19937::default_seeder{rd()}})
      .separate()
      .run("pxt xrsr", pxart::xrsr128p{rd})
      .run("xrsr  x2", pxart::xrsr128p{rd}, pxart::xrsr128p{rd})
      .run("px xs256", pxart::simd256::xrsr128p{rd})
      .run("xs256 x2", pxart::simd256::xrsr128p{rd},
           pxart::simd256::xrsr128p{rd})
      .run("px xs128", pxart::simd128::xrsr128p{rd})
      .run("xs128 x2", pxart::simd128::xrsr128p{rd},
           pxart::simd128::xrsr128p{rd})
      .separate()
      .run("pxt msws", pxart::msws{rd})
      .run("msws  x2", pxart::msws{rd}, pxart::msws{rd})
      .run("px mw256", pxart::simd256::msws{rd})
      .run("mw256 x2", pxart::simd256::msws{rd}, pxart::simd256::msws{rd})
      .run("px mw128", pxart::simd128::msws{rd})
      .run("mw128 x2", pxart::simd128::msws{rd}, pxart::simd128::msws{rd});
}