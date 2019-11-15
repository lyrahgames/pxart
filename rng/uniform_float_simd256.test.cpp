#include <iostream>
#include <pxart/uniform_real_distribution.hpp>
#include <random>

using namespace std;

int main() {
  random_device rd{};

  const int bins = 50;
  int histogram[bins]{};
  const int n = 1000000;

  for (int i = 0; i < n; ++i) {
    const auto rnd =
        _mm256_set_epi32(rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd());
    const auto number = pxart::uniform_double_from_32(rnd);
    for (int j = 0; j < 8; ++j) {
      const auto tmp = reinterpret_cast<const double*>(&number)[j];
      ++histogram[static_cast<int>(tmp * bins)];
    }
  }

  int max = 0;
  for (int i = 0; i < bins; ++i) max = std::max(max, histogram[i]);

  for (int i = 0; i < bins; ++i) {
    const auto count = static_cast<int>(100.0f * histogram[i] / max);
    for (int j = 0; j < count; ++j) cout.put('*');
    cout << "\n";
  }
}