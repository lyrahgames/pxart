#include <algorithm>
#include <chrono>
#include <cstdio>
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

  // freopen(nullptr, "wb", stdout);
  // while (1) {
  //   uint32_t value = rng();
  //   fwrite((void*)&value, sizeof(value), 1, stdout);
  // }

  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  // while (true) {
  //   const uint32_t sample = rng();
  //   cout.write(reinterpret_cast<const char*>(&sample), sizeof(sample));
  // }

  cout.setf(ios::unitbuf);
  constexpr int c = 1 << 20;
  vector<uint32_t> cache1(c), cache2(c);
  generate(cache1.begin(), cache1.end(), ref(rng));
  while (true) {
    swap(cache1, cache2);
    auto handle = async(launch::async, [&cache2]() {
      cout.write(reinterpret_cast<const char*>(cache2.data()),
                 sizeof(uint32_t) * cache2.size());
      // fwrite((void*)cache2.data(), sizeof(uint32_t), cache2.size(), stdout);
    });
    generate(cache1.begin(), cache1.end(), ref(rng));
    handle.wait();
  }
}