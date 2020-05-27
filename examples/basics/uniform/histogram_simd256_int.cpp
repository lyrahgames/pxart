#include <array>
#include <iomanip>
#include <iostream>
#include <pxart/pxart.hpp>
#include <random>

int main() {
  using namespace std;

#ifdef __AVX2__

  constexpr int samples = 10'000'000;
  constexpr int buckets = 17;
  constexpr int offset = -3;
  int histogram[buckets]{};

  pxart::simd256::xrsr128p rng{random_device{}};

  using type = int;
  constexpr auto size = sizeof(rng()) / sizeof(type);

  for (int i = 0; i < samples; i += size) {
    const auto vrnd = pxart::uniform<type>(rng, offset, offset + buckets - 1);
    const auto srnd = pxart::pun_cast<array<type, size>>(vrnd);
    for (int j = 0; j < size; ++j) ++histogram[srnd[j] - offset];
  }

  int max_count = 0;
  for (int i = 0; i < buckets; ++i) max_count = max(max_count, histogram[i]);

  for (int i = 0; i < buckets; ++i)
    cout << setw(5) << i + offset << ":" << setw(10) << histogram[i] << '\t'
         << setfill('*') << setw(30.0f * histogram[i] / max_count) << '\n'
         << setfill(' ');

#else

  cout << "Example can only be used with AVX2 instruction set.\n";

#endif  // __AVX2__
}