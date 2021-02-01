#include <random>
//
#include <perfevent/perfevent.hpp>
//
#include <pxart/distribution/uniform.hpp>
#include <pxart/generator/minstd_rand.hpp>
#include <pxart/generator/mt19937.hpp>

std::random_device rd;

struct benchmark {
  BenchmarkParameters params;
  size_t n = 100'000'000;

#define RUN(X) run(#X, X{rd})

  PXART_TEMPLATE(pxart::generic::random_bit_generator, G)
  benchmark& run(const char* name, G&& g) {
    params.setParam("                 name", name);
    float result = 0;
    {
      PerfEventBlock _(n, params, true);
      for (size_t i = 0; i < n; ++i) result += pxart::uniform<float>(g, -1, 1);
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