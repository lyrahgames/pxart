#include "monte_carlo_pi.hpp"

#include <cmath>
#include <iostream>
#include <random>
#include <sstream>

template <typename Real>
constexpr Real pi = 3.1415926535897932384626433;

using Real = double;
using namespace std;

int main(int argc, char** argv) {
  if (argc != 2) {
    cout << "usage: " << argv[0] << " <max sample count>\n";
    return -1;
  }

  stringstream input{argv[1]};
  int n;
  input >> n;

  random_device rng{};

  cout << "#samples\testimation\terror\trelative error\n";
  for (int i = 10; i <= n; i *= 10) {
    const auto monte_carlo_pi = monte_carlo::pi<Real>(rng, i);
    const auto error = abs(monte_carlo_pi - pi<Real>);
    const auto relative_error = error / pi<Real>;
    cout << i << "\t" << monte_carlo_pi << "\t" << error << "\t"
         << relative_error << "\n";
  }
}