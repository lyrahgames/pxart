#include <iomanip>
#include <iostream>
#include <random>
//
#include <pxart/mt19937.hpp>
#include <pxart/uniform.hpp>

using namespace std;

int main() {
  // Initialize pxart PRNG.
  pxart::mt19937 rng{random_device{}};
  // Print some pseudorandom numbers.
  for (size_t i = 0; i < 10; ++i) {
    // Generate uniformly distributed single-precision floating-point number
    // in the interval [0, 1).
    const auto random1 = uniform<float>(rng);
    // Generate uniformly distributed double-precision floating-point number
    // in the interval [-1.5, 1.0).
    const auto random2 = uniform(rng, -1.5, 1.0);

    cout << setw(20) << random1 << setw(20) << random2 << '\n';
  }
}