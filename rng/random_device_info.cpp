#include <iostream>
#include <random>
#include <vector>

#include <librng/sample_analyzer.hpp>

using namespace std;

int main() {
  random_device rd{};
  mt19937 rng{rd()};
  uniform_int_distribution<> distribution{0, 100};
  // uniform_real_distribution<float> distribution{0, 2};
  // normal_distribution<> distribution{};

  vector<int> data{};
  for (long i = 0; i < 1000; ++i) data.push_back(distribution(rng));
  rng::sample_analyzer info{data};

  cout << "random_device information:\n"
       << "entropy = " << rd.entropy() << '\n'
       << "min = " << rd.min() << '\n'
       << "max = " << rd.max() << '\n';

  cout << info << '\n';
}