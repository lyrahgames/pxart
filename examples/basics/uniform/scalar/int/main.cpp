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
    // Generate uniformly distributed integral numbers of type int
    // in the interval [-12, 25].
    const auto random1 = uniform<int>(rng, -12, 25);
    // Generate uniformly distributed characters
    // in the interval ['A', 'Z'].
    const auto random2 = uniform<char>(rng, 'A', 'Z');

    cout << setw(20) << random1 << setw(20) << random2 << '\n';
  }
}