#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

using namespace std;

template <typename Real>
constexpr Real pi = 3.1415926535897932384626433;

using Real = double;

int main(int argc, char** argv) {
  string file_path{"monte_carlo_pi_plot.txt"};

  if (argc < 2 || argc > 3) {
    cout << "usage: " << argv[0] << " <sample count> [<file path>]\n";
    return -1;
  } else if (argc == 3) {
    file_path = argv[2];
  }

  stringstream input{argv[1]};
  int n;
  input >> n;

  mt19937 rng{random_device{}()};
  uniform_real_distribution<Real> dist{};

  fstream file{file_path, ios::out};

  int in_count = 0;
  Real monte_carlo_pi{};
  Real error{};
  Real rel_error{};
  int step = 20;
  for (int i = 1; i <= n; ++i) {
    const auto x = dist(rng);
    const auto y = dist(rng);
    if (x * x + y * y <= 1) ++in_count;

    if (i > step) {
      monte_carlo_pi = static_cast<Real>(in_count) / i * 4;
      error = abs(monte_carlo_pi - pi<Real>);
      rel_error = error / pi<Real>;

      file << i << "\t" << monte_carlo_pi << "\t" << error << "\t" << rel_error
           << "\n";

      step = 1.15 * static_cast<Real>(step);
    }
  }
}