#include <celero/Celero.h>

#include <PerfEvent.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <iostream>
// #include <pxart/middle_square_weyl_engine.hpp>
#include <pxart/monte_carlo/pi.hpp>
#include <pxart/msws.hpp>
#include <pxart/mt19937.hpp>
#include <pxart/rdrand_engine.hpp>
#include <pxart/simd128/mt19937.hpp>
#include <pxart/simd256/msws.hpp>
#include <pxart/simd256/mt19937.hpp>
#include <pxart/xoroshiro128plus.hpp>
#include <pxart/xoroshiro128plus_simd256.hpp>
#include <random>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
  if (2 != argc) {
    cout << "usage:\n" << argv[0] << " <number of elements>\n";
    return -1;
  }

  stringstream input{argv[1]};
  uint64_t n;
  input >> n;

  random_device rd{};
  float pi{};

  BenchmarkParameters params;
  // params.setParam("n", to_string(n));
  // {
  //   params.setParam("name", "std::random_device");
  //   PerfEventBlock e(n, params, true);
  //   celero::DoNotOptimizeAway(pi = pxart::monte_carlo::pi<float>(rd, n));
  // }
  // cout << "pi = " << pi << "\n";
  // {
  //   params.setParam("name", "pxart::rdrand_engine");
  //   PerfEventBlock e(n, params, true);
  //   pxart::rdrand_engine rng{};
  //   celero::DoNotOptimizeAway(pi = pxart::monte_carlo::pi<float>(rng, n));
  // }
  // cout << "pi = " << pi << "\n";
  // {
  //   params.setParam("name", "std::minstd_rand");
  //   minstd_rand lcg_rng{rd()};
  //   PerfEventBlock e(n, params, true);
  //   celero::DoNotOptimizeAway(pxart::monte_carlo::pi<float>(lcg_rng, n));
  // }
  // {
  //   params.setParam("name", "std::minstd_rand x 2");
  //   minstd_rand rng1{rd()};
  //   minstd_rand rng2{rd()};
  //   PerfEventBlock e(n, params, true);
  //   celero::DoNotOptimizeAway(pxart::monte_carlo::pi<float>(rng1, rng2, n));
  // }
  {
    params.setParam("name", "std::mt19937");
    mt19937 mt_rng{rd()};
    PerfEventBlock e(n, params, true);
    celero::DoNotOptimizeAway(pi = pxart::monte_carlo::pi<float>(mt_rng, n));
  }
  cout << "pi = " << pi << "\n";
  {
    params.setParam("name", "pxart::mt19937");
    pxart::mt19937 rng{rd};
    PerfEventBlock e(n, params, true);
    celero::DoNotOptimizeAway(pi = pxart::monte_carlo::pi<float>(rng, n));
  }
  cout << "pi = " << pi << "\n";
  {
    params.setParam("name", "boost::mt19937");
    boost::random::mt19937 rng{rd()};
    PerfEventBlock e(n, params, true);
    celero::DoNotOptimizeAway(pi = pxart::monte_carlo::pi<float>(rng, n));
  }
  cout << "pi = " << pi << "\n";
  {
    params.setParam("name", "pxart::xrsr128p");
    pxart::xrsr128p rng{rd};
    PerfEventBlock e(n, params, true);
    celero::DoNotOptimizeAway(pi = pxart::monte_carlo::pi<float>(rng, n));
  }
  cout << "pi = " << pi << "\n";
  {
    params.setParam("name", "pxart::xrsr128p x 2");
    pxart::xrsr128p rng1{rd};
    pxart::xrsr128p rng2{rd};
    PerfEventBlock e(n, params, true);
    celero::DoNotOptimizeAway(pi =
                                  pxart::monte_carlo::pi<float>(rng1, rng2, n));
  }
  cout << "pi = " << pi << "\n";
  {
    params.setParam("name", "std::mt19937 simd::pi");
    std::mt19937 rng{rd()};
    PerfEventBlock e(n, params, true);
    celero::DoNotOptimizeAway(pi = pxart::monte_carlo::simd::pi(rng, n));
  }
  cout << "pi = " << pi << "\n";
  // {
  //   params.setParam("name", "pxart::xrsr128p simd::pi");
  //   pxart::xrsr128p rng{rd};
  //   PerfEventBlock e(n, params, true);
  //   celero::DoNotOptimizeAway(pi = pxart::monte_carlo::simd::pi(rng, n));
  // }
  // cout << "pi = " << pi << "\n";
  // {
  //   params.setParam("name", "pxart::xrsr128p simd::pi x 2");
  //   pxart::xrsr128p rng1{rd};
  //   pxart::xrsr128p rng2{rd};
  //   PerfEventBlock e(n, params, true);
  //   celero::DoNotOptimizeAway(pi = pxart::monte_carlo::simd::pi(rng1, rng2,
  //   n));
  // }
  // cout << "pi = " << pi << "\n";
  {
    params.setParam("name", "pxart::xs128p_simd256");
    pxart::xs128p_simd256 rng{rd};
    PerfEventBlock e(n, params, true);
    celero::DoNotOptimizeAway(pi = pxart::monte_carlo::simd::vprng::pi(rng, n));
  }
  cout << "pi = " << pi << "\n";
  {
    params.setParam("name", "pxart::xs128p_simd256 x 2");
    pxart::xs128p_simd256 rng1{rd};
    pxart::xs128p_simd256 rng2{rd};
    PerfEventBlock e(n, params, true);
    celero::DoNotOptimizeAway(
        pi = pxart::monte_carlo::simd::vprng::pi(rng1, rng2, n));
  }
  cout << "pi = " << pi << "\n";
  {
    params.setParam("name", "pxart::simd256::mt19937");
    pxart::simd256::mt19937 rng{rd};
    PerfEventBlock e(n, params, true);
    celero::DoNotOptimizeAway(pi = pxart::monte_carlo::simd::vprng::pi(rng, n));
  }
  cout << "pi = " << pi << "\n";
  {
    params.setParam("name", "pxart::simd128::mt19937");
    pxart::simd128::mt19937 rng{rd};
    PerfEventBlock e(n, params, true);
    celero::DoNotOptimizeAway(pi = pxart::monte_carlo::simd128::pi(rng, n));
  }
  cout << "pi = " << pi << "\n";
  // {
  //   params.setParam("name", "pxart::middle_square_weyl_engine");
  //   pxart::middle_square_weyl_engine rng{rd};
  //   PerfEventBlock e(n, params, true);
  //   celero::DoNotOptimizeAway(pi = pxart::monte_carlo::pi<float>(rng, n));
  // }
  // cout << "pi = " << pi << "\n";
  // {
  //   params.setParam("name", "pxart::simd256::msws");
  //   pxart::simd256::msws rng{rd};
  //   PerfEventBlock e(n, params, true);
  //   celero::DoNotOptimizeAway(pi = pxart::monte_carlo::simd::vprng::pi(rng,
  //   n));
  // }
  // cout << "pi = " << pi << "\n";
  // {
  //   params.setParam("name", "pxart::simd128::msws");
  //   pxart::simd128::msws rng{rd};
  //   PerfEventBlock e(n, params, true);
  //   celero::DoNotOptimizeAway(pi = pxart::monte_carlo::simd128::pi(rng, n));
  // }
  // cout << "pi = " << pi << "\n";
  {
    params.setParam("name", "pxart::msws");
    pxart::msws rng{rd};
    PerfEventBlock e(n, params, true);
    celero::DoNotOptimizeAway(pi = pxart::monte_carlo::pi<float>(rng, n));
  }
  cout << "pi = " << pi << "\n";
  {
    params.setParam("name", "pxart::simd256::msws");
    pxart::simd256::msws rng{rd};
    PerfEventBlock e(n, params, true);
    celero::DoNotOptimizeAway(pi = pxart::monte_carlo::simd::vprng::pi(rng, n));
  }
  cout << "pi = " << pi << "\n";
}
