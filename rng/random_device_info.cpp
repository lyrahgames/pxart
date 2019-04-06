#include <iostream>
#include <random>
#include <vector>

#include <librng/sample_analyzer.hpp>

using namespace std;

int main() {
  random_device rd{};
  mt19937 rng{rd()};
  uniform_int_distribution<> distribution{0, 23};
  // normal_distribution<> distribution{0, 100};

  vector<decltype(distribution)::result_type> data{};
  for (long i = 0; i < 1000000; ++i) data.push_back(distribution(rng));
  rng::sample_analyzer info{data};

  cout << "random_device information:\n"
       << "entropy = " << rd.entropy() << '\n'
       << "min = " << rd.min() << '\n'
       << "max = " << rd.max() << '\n';

  cout << info << '\n';
}