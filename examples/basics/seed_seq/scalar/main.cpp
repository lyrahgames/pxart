#include <iomanip>
#include <iostream>
//
#include <pxart/mt19937.hpp>
#include <pxart/seed_seq.hpp>

using namespace std;

int main() {
  // Construct PRNG by seeding it with seed_seq_8x32 which itself uses four
  // fixed seed values.
  pxart::seed_seq_8x32 seeder{1, 2, 3, 4};
  pxart::mt19937 rng{seeder};

  // Return some pseudorandom numbers.
  for (int i = 0; i < 10; ++i) cout << setw(15) << rng() << '\n';
}