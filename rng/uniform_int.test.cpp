#include <iostream>
#include <pxart/uniform_int_distribution.hpp>
#include <random>

using namespace std;

int main() {
  const int n = 100;
  const int s = 1023;

  random_device rd{};

  for (int i = 0; i < n; i += 8) {
    const auto sv =
        _mm256_set_epi32(rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd());
    const auto rndv256 = pxart::uniform_int(sv, s);
    for (int j = 0; j < 8; ++j) {
      const auto rnds =
          pxart::uniform_int(reinterpret_cast<const uint32_t*>(&sv)[j], s);
      const auto rndv = reinterpret_cast<const uint32_t*>(&rndv256)[j];
      cout << rnds << "\t-\t" << rndv << "\t=\t" << rnds - rndv << "\n";
    }
  }
}