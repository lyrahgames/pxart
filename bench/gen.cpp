#include <algorithm>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

#include <boost/random/mersenne_twister.hpp>

#include <pxart/mt19937.hpp>

#include <celero/Celero.h>
#include <PerfEvent.hpp>

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
  mt19937 rng{rd()};
  mt19937 rng2{rd()};

  BenchmarkParameters params;
  params.setParam("n", to_string(n));
  {
    params.setParam("name", "std::mt19937");
    PerfEventBlock e(n, params, true);
    for (auto i = n; i > 0; --i) {
      auto tmp = rng();
    }
  }
  {
    params.setParam("name", "std::mt19937 2");
    PerfEventBlock _(n, params, false);
    for (auto i = n; i > 0; i -= 2) {
      auto tmp = rng();
      auto tmp2 = rng2();
    }
  }
  {
    boost::random::mt19937 rng{rd()};
    params.setParam("name", "boost::mt19937");
    PerfEventBlock _(n, params, false);
    for (auto i = n; i > 0; --i) {
      celero::DoNotOptimizeAway(rng());
    }
  }
  {
    pxart::mt19937 rng{};
    params.setParam("name", "pxart::mt19937");
    PerfEventBlock _(n, params, false);
    for (auto i = n; i > 0; --i) {
      celero::DoNotOptimizeAway(rng());
    }
  }
}
