#include <algorithm>
#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <random>
#include <thread>
#include <utility>
#include <vector>

using namespace std;

int main() {
  mt19937 rng{random_device{}()};

  // while (true) {
  //   const uint32_t sample = rng();
  //   cout.write(reinterpret_cast<const char*>(&sample), sizeof(sample));
  // }

  constexpr int c = 100'000;
  vector<uint32_t> cache1(c);
  auto cache2 = cache1;
  generate(cache1.begin(), cache1.end(), ref(rng));
  while (true) {
    swap(cache1, cache2);
    auto handle = async(launch::async, [&cache2]() {
      cout.write(reinterpret_cast<const char*>(cache2.data()),
                 sizeof(uint32_t) * cache2.size());
    });
    generate(cache1.begin(), cache1.end(), ref(rng));
    handle.wait();
  }
}