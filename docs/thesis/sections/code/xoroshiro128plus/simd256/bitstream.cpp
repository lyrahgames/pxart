#include <cstdio>
#include <iostream>
#include <pxart/simd256/xoroshiro128plus.hpp>
#include <random>

using namespace std;

int main() {
  // Speed up the output of bits by not syncing
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  // Initialize RNG.
  pxart::simd256::xrsr128p rng{random_device{}};
  // Write the binary representation of generated
  // random numbers into the bitstream.
  while (true) {
    const auto sample = rng();
    cout.write(reinterpret_cast<const char*>(&sample), sizeof(sample));
  }
}