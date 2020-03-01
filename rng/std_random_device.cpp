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
  random_device rng{};

  // while (true) {
  //   const auto sample = rng();
  //   cout.write(reinterpret_cast<const char*>(&sample), sizeof(sample));
  // }

  constexpr int c = 1'000'000;
  vector<random_device::result_type> cache1(c);
  auto cache2 = cache1;
  generate(cache1.begin(), cache1.end(), ref(rng));
  while (true) {
    swap(cache1, cache2);
    auto handle = async(launch::async, [&cache2]() {
      cout.write(reinterpret_cast<const char*>(cache2.data()),
                 sizeof(random_device::result_type) * cache2.size());
    });
    generate(cache1.begin(), cache1.end(), ref(rng));
    handle.wait();
  }
}