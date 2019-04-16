#include <cstdint>
#include <iostream>
#include <random>

#include <librng/middle_square_engine.hpp>

using namespace std;

int main() {
  mt19937 rng{random_device{}()};
  // rng::middle_square_engine rng{};
  while (true) {
    const uint32_t sample = rng();
    cout.write(reinterpret_cast<const char*>(&sample), sizeof(sample));
  }
}