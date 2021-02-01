#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
//
#ifndef PXART_BENCHMARKS_DISABLE_PERFEVENT
#include <perfevent/perfevent.hpp>
#endif
//
#include <pxart/distribution/uniform.hpp>
#include <pxart/generator/minstd_rand.hpp>
#include <pxart/generator/mt19937.hpp>

using namespace std;

random_device rd;

struct benchmark {
  size_t n = 100'000'000;
#ifndef PXART_BENCHMARKS_DISABLE_PERFEVENT
  BenchmarkParameters params;
#endif

#define RUN(X) run(#X, X{rd})

  PXART_TEMPLATE(pxart::generic::random_bit_generator, G)
  benchmark& run(const char* name, G&& g) {
#ifndef PXART_BENCHMARKS_DISABLE_PERFEVENT
    params.setParam("                 name", name);
#endif
    float result = 0;
    {
#ifndef PXART_BENCHMARKS_DISABLE_PERFEVENT
      PerfEventBlock _(n, params, true);
#else
      const auto start = chrono::high_resolution_clock::now();
#endif

      for (size_t i = 0; i < n; ++i) result += pxart::uniform<float>(g, -1, 1);

#ifdef PXART_BENCHMARKS_DISABLE_PERFEVENT
      const auto end = chrono::high_resolution_clock::now();
      const auto time = chrono::duration<double>(end - start).count();
      cout << left << setw(30) << name << right << setw(20) << time << " s\n"
           << flush;
#endif
    }
    volatile auto do_not_optimize_away = result;
    return *this;
  }
};

int main() {
  benchmark{}  //
      .RUN(pxart::minstd_rand)
      .RUN(pxart::mt19937)
      //
      ;
}