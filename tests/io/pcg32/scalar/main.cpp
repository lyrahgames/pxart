#include <iomanip>
#include <iostream>
//
#include <pxart/pcg32.hpp>

using namespace std;

int main() {
  // Default initialize PRNG.
  pxart::pcg32 rng{};
  // Generate pseudorandom numbers.
  for (size_t i = 0; i < 20; ++i) {
    for (size_t j = 0; j < 5; ++j) cout << setw(20) << rng();
    cout << '\n';
  }
}