#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>
//
#include <boost/random/mersenne_twister.hpp>
//
#include <pxart/generator/minstd_rand.hpp>
#include <pxart/generator/mt19937.hpp>
#include <pxart/meta.hpp>
//
#ifndef PXART_BENCHMARKS_DISABLE_GNUPLOT
#include <lyrahgames/gnuplot_pipe.hpp>
#endif

using namespace std;

const size_t n = 1'000'000;
const size_t run_count = 100;

random_device rd{};
volatile uint64_t do_not_optimize_away;

PXART_TEMPLATE(pxart::generic::random_bit_generator, G,
               (pxart::generic::seedable_by<G, random_device>))
inline auto seeded_generator() { return G{rd}; }
PXART_TEMPLATE(pxart::generic::random_bit_generator, G,
               (!pxart::generic::seedable_by<G, random_device>))
inline auto seeded_generator() { return G{rd()}; }
template <>
inline auto seeded_generator<boost::random::mt19937>() {
  return boost::random::mt19937{rd()};
}

PXART_TEMPLATE(pxart::generic::random_bit_generator, G)
double performance_runner() {
  auto g = seeded_generator<G>();
  pxart::meta::result<G> result = 0;

  // Warm-Up
  for (size_t i = 0; i < n; ++i) result += g();

  const auto samples = n;

  const auto start = chrono::high_resolution_clock::now();

  for (size_t i = 0; i < samples; ++i) result += g();

  const auto end = chrono::high_resolution_clock::now();
  const auto time = chrono::duration<double>(end - start).count();

  do_not_optimize_away = result;
  // cout << result;

  const auto performance = n * 4 / time / (1024.0 * 1024.0);
  return performance;
}

struct runner {
  const char* name;
  double (*run)();
  double max = 0;
  double mean = 0;
  double stddev = 0;
};
vector<runner> runners{};

int main() {
  runners.push_back({"pxart::mt19937", performance_runner<pxart::mt19937>});
  runners.push_back({"std::mt19937", performance_runner<std::mt19937>});
  runners.push_back(
      {"boost::random::mt19937", performance_runner<boost::random::mt19937>});
  runners.push_back(
      {"pxart::minstd_rand", performance_runner<pxart::minstd_rand>});
  runners.push_back({"std::minstd_rand", performance_runner<std::minstd_rand>});

  vector<size_t> indices(runners.size());
  iota(begin(indices), end(indices), 0);

  const auto samples = run_count;

  for (size_t k = 0; k < samples; ++k) {
    shuffle(begin(indices), end(indices), rd);
    for (auto i : indices) {
      const auto performance = runners[i].run();
      runners[i].mean += performance;
      runners[i].stddev += performance * performance;
      runners[i].max = max(runners[i].max, performance);
      // cout << setw(30) << left << runners[i].name << setw(25) << right
      //      << performance << " MiB/s\n"
      //      << flush;
    }
    // cout << '\n';
  }
  // cout << '\n';

  for (size_t i = 0; i < runners.size(); ++i) {
    runners[i].mean /= run_count;
    runners[i].stddev = sqrt(
        (runners[i].stddev - run_count * runners[i].mean * runners[i].mean) /
        (run_count - 1));
  }

  for (size_t i = 0; i < runners.size(); ++i) {
    cout << setw(30) << left << runners[i].name << right << setw(25)
         << runners[i].max << " MiB/s" << setw(25) << runners[i].mean
         << " MiB/s" << setw(25) << runners[i].stddev << " MiB/s" << '\n';
  }

#ifndef PXART_BENCHMARKS_DISABLE_GNUPLOT
  fstream file{"performance.txt", ios::out};
  for (size_t i = 0; i < runners.size(); ++i)
    file << '"' << runners[i].name << '"' << '\t' << runners[i].max << '\t'
         << runners[i].mean << '\t' << runners[i].stddev << '\n';
  file << flush;
  lyrahgames::gnuplot_pipe plot{};
  plot << "unset key\n"
          "set xr [-1:5]\n"
          "set yr [0:1000 < *]\n"
          "set yl \"Performance [MiB/s]\"\n"
          "set xtics rotate by 20 right\n"
          "set xtics nomirror scale 0\n"
          "plot 'performance.txt' u 0:2:(0.7):xtic(1) w boxes lc rgb '#222222' "
          "fs solid 0.2\n";
#endif  // PXART_BENCHMARKS_DISABLE_GNUPLOT
}