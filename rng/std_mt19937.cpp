#include <cstdint>
#include <iostream>
#include <random>

using namespace std;

int main() {
  mt19937 rng{random_device{}()};
  while (true) {
    const uint32_t sample = rng();
    cout.write(reinterpret_cast<const char*>(&sample), sizeof(sample));
  }
}