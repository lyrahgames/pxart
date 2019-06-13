#include <algorithm>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

#include <PerfEvent.hpp>

using namespace std;

int main(int argc, char** argv) {
  if (2 != argc) {
    cout << "usage:\n" << argv[0] << " <number of elements>\n";
    return -1;
  }

  stringstream input{argv[1]};
  uint64_t n;
  input >> n;

  mt19937 rng{random_device{}()};
  vector<mt19937::result_type> buffer(n);

  BenchmarkParameters params;
  params.setParam("name", "Dummy Benchmark");
  {
    PerfEventBlock e(n, params, true);
    generate(buffer.begin(), buffer.end(), rng);
  }
}
