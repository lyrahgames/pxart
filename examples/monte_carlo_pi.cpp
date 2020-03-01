#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

using namespace std;

int main(int argc, char** argv) {
  string file_path{"monte_carlo_pi_data"};

  if (argc < 2 || argc > 3) {
    cout << "usage: " << argv[0] << " <sample count> [<base file name>]\n";
    return -1;
  } else if (argc == 3) {
    file_path = argv[2];
  }

  stringstream input{argv[1]};
  int n;
  input >> n;

  using Real = double;

  mt19937 rng{random_device{}()};
  uniform_real_distribution<Real> dist{};

  fstream file_in{file_path + "_in.txt", ios::out};
  fstream file_out{file_path + "_out.txt", ios::out};

  int in_count = 0;
  for (int i = 0; i < n; ++i) {
    const auto x = dist(rng);
    const auto y = dist(rng);

    if (x * x + y * y <= 1) {
      ++in_count;
      file_in << x << "\t" << y << "\n";
    } else {
      file_out << x << "\t" << y << "\n";
    }
  }

  const auto monte_carlo_pi = static_cast<Real>(in_count) / n * 4;

  cout << "monte_carlo_pi = " << monte_carlo_pi << "\n"
       << "points in circle = " << in_count << "\n"
       << "sample points = " << n << "\n";
}