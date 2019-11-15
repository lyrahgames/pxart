#include <iostream>
#include <pxart/uniform_real_distribution.hpp>
#include <random>

using namespace std;

int main() {
  random_device rd{};

  const int bins = 50;
  int histogram[bins]{};
  const int n = 10000000;

  for (int i = 0; i < n; ++i) {
    ++histogram[static_cast<int>(pxart::uniform(rd(), 0.125f, 0.655f) * bins)];
  }

  int max = 0;
  for (int i = 0; i < bins; ++i) max = std::max(max, histogram[i]);

  for (int i = 0; i < bins; ++i) {
    const auto count = static_cast<int>(100.0f * histogram[i] / max);
    for (int j = 0; j < count; ++j) cout.put('*');
    cout << "\n";
  }
}