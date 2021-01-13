#include <iomanip>
#include <iostream>
#include <random>
//
#include <pxart/mt19937.hpp>

using namespace std;

int main() {
  // Best case: Generate truly random seed.
  const auto seed = std::random_device{}();

  // Generate std PRNG to compare against.
  std::mt19937 std_rng{seed};
  // Generate pxart PRNG with the default seeder and the same seed.
  pxart::mt19937::default_seeder seeder{seed};
  pxart::mt19937 rng{seeder};

  // Print some pseudorandom numbers of the standard generator and the pxart
  // generator to compare them and show that they are equal.
  for (size_t i = 0; i < 10; ++i)
    cout << setw(20) << std_rng() << setw(20) << rng() << '\n';
}