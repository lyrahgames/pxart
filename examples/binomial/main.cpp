#include <iomanip>
#include <iostream>
#include <random>
#include <string>
//
#include <pxart/distribution/binomial.hpp>
#include <pxart/generator/mt19937.hpp>

using namespace std;

int main() {
  pxart::mt19937 rng{std::random_device{}};

  using real = float;
  const unsigned int n = 20;
  const real p = 0.2f;
  const size_t samples = 100'000'000;

  pxart::binomial binomial{n, p};

  size_t histogram[n + 1]{};
  for (size_t i = 0; i < samples; ++i) ++histogram[binomial(rng)];

  for (size_t i = 0; i <= n; ++i) {
    const auto tmp = histogram[i] / real{samples};
    cout << setw(5) << i << ": " << setw(10) << setprecision(6) << tmp << ": "
         << string(100 * tmp, '*') << '\n';
  }
}