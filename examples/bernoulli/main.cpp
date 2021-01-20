#include <iomanip>
#include <iostream>
#include <random>
#include <string>
//
#include <pxart/distribution/bernoulli.hpp>
#include <pxart/generator/mt19937.hpp>

using namespace std;

int main() {
  pxart::mt19937 rng{std::random_device{}};

  using real = float;
  const real p = 0.4f;
  const size_t n = 100'000;

  size_t histogram[2]{};
  for (size_t i = 0; i < n; ++i) ++histogram[pxart::bernoulli<int>(rng, p)];

  cout << setw(20) << histogram[0] / real{n} << '\n'
       << setw(20) << histogram[1] / real{n} << '\n';
}