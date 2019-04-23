#include <chrono>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
  constexpr int n = 10'000'000;
  constexpr int c = 10'000'000;

  random_device rng{};

  const auto start = chrono::high_resolution_clock::now();
  for (int i = 0; i < n; ++i) {
    const auto sample = rng();
    cout.write(reinterpret_cast<const char*>(&sample), sizeof(sample));
  }
  const auto end = chrono::high_resolution_clock::now();

  const auto cache_start = chrono::high_resolution_clock::now();
  vector<random_device::result_type> cache(c);
  for (int i = 0; i < n; i += c) {
    for (int j = 0; j < c; ++j) {
      cache[j] = rng();
    }
    cout.write(reinterpret_cast<const char*>(cache.data()),
               sizeof(random_device::result_type) * c);
  }
  const auto cache_end = chrono::high_resolution_clock::now();

  cout << "\n\nserial time = " << chrono::duration<float>(end - start).count()
       << '\n'
       << "cache time = "
       << chrono::duration<float>(cache_end - cache_start).count() << '\n';
}