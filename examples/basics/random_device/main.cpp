#include <iomanip>
#include <iostream>
#include <random>
//
#include <pxart/mt19937.hpp>

using namespace std;

int main() {
  // Create pxart PRNG and seed it by using std::random_device.
  pxart::mt19937 rng{std::random_device{}};
  // Print some random numbers.
  for (size_t i = 0; i < 10; ++i) cout << setw(20) << rng() << '\n';
}