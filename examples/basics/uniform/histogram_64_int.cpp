#include <iomanip>
#include <iostream>
#include <pxart/pxart.hpp>
#include <random>

int main() {
  using namespace std;
  constexpr int samples = 10'000'000;
  constexpr int buckets = 17;
  constexpr int offset = -3;
  int histogram[buckets]{};

  pxart::xrsr128p rng{random_device{}};

  for (int i = 0; i < samples; ++i)
    ++histogram[pxart::uniform(rng, offset, offset + buckets - 1) - offset];

  int max_count = 0;
  for (int i = 0; i < buckets; ++i) max_count = max(max_count, histogram[i]);

  for (int i = 0; i < buckets; ++i)
    cout << setw(5) << i + offset << ":" << setw(10) << histogram[i] << '\t'
         << setfill('*') << setw(30.0f * histogram[i] / max_count) << '\n'
         << setfill(' ');
}