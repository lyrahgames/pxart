#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>
#include <vector>
//
#ifndef PXART_BENCHMARKS_DISABLE_BOOST
#include <boost/random/mersenne_twister.hpp>
#endif
//
#include <pxart/distribution/uniform.hpp>
#include <pxart/generator/minstd_rand.hpp>
#include <pxart/generator/msws.hpp>
#include <pxart/generator/mt19937.hpp>
#include <pxart/meta.hpp>
//
#ifndef PXART_BENCHMARKS_DISABLE_GNUPLOT
#include <lyrahgames/gnuplot_pipe.hpp>
#endif

using namespace std;

random_device rd{};

PXART_TEMPLATE(pxart::generic::random_bit_generator, G,
               (pxart::generic::seedable_by<G, random_device>))
inline auto seeded_generator() { return G{rd}; }
PXART_TEMPLATE(pxart::generic::random_bit_generator, G,
               (!pxart::generic::seedable_by<G, random_device>))
inline auto seeded_generator() { return G{rd()}; }

#ifndef PXART_BENCHMARKS_DISABLE_BOOST
template <>
inline auto seeded_generator<boost::random::mt19937>() {
  return boost::random::mt19937{rd()};
}
#endif  // PXART_BENCHMARKS_DISABLE_BOOST

PXART_TEMPLATE(pxart::generic::random_bit_generator, G)
static inline double benchmark_performance(const size_t samples) {
  auto g = seeded_generator<G>();
  // Warm-Up
  {
    pxart::meta::result<G> result = 0;
    for (size_t i = 0; i < samples; ++i) result += g();
    volatile auto do_not_optimize_away = result;
    cout << result << '\t';
  }
  // Measurement
  float result;
  const auto start = chrono::high_resolution_clock::now();
  {
    size_t interior = 0;
    for (size_t i = 0; i < samples; ++i) {
      const auto x = pxart::uniform<float>(g);
      const auto y = pxart::uniform<float>(g);
      interior += ((x * x + y * y) < 1);
    }
    result = 4.0f * interior / samples;
  }
  const auto end = chrono::high_resolution_clock::now();
  const auto time = chrono::duration<double>(end - start).count();

  cout << result << '\t';
  if (abs(M_PI - result) > 4.0 / sqrt(samples)) {
    cout << "Possible failure!";
  }
  cout << '\n';

  const auto performance = samples / time / (1000.0 * 1000.0);
  return performance;
}

struct benchmark {
  size_t n = 100'000'000;
  size_t run_count = 10;

  struct runner {
    const char* name;
    double (*run)(const size_t);
    double max = 0;
    double mean = 0;
    double stddev = 0;
  };

  vector<runner> runners{};

  benchmark& add(const char* name, double (*run)(const size_t)) {
    runners.push_back({name, run});
    return *this;
  }
#define ADD(X) add(#X, benchmark_performance<X>)

  benchmark& run() {
    vector<size_t> indices(runners.size());
    iota(begin(indices), end(indices), 0);

    for (size_t k = 0; k < run_count; ++k) {
      shuffle(begin(indices), end(indices), rd);
      // #pragma omp parallel for
      for (auto i : indices) {
        // for (size_t p = 0; p < indices.size(); ++p) {
        //   const auto i = indices[p];
        const auto performance = runners[i].run(n);
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
    return *this;
  }

  friend ostream& operator<<(ostream& os, benchmark& d) {
    os << setw(30) << "Monte Carlo π Benchmark" << setw(30)
       << "samples/run = " << d.n << ", runs = " << d.run_count << '\n'
       << string(105, '-') << '\n'
       << left << setw(30) << "Name" << right << setw(25) << "Max. [MSamples/s]"
       << setw(25) << "Mean [MSamples/s]" << setw(25)
       << "Stddev. [MSamples/s]\n"
       << string(105, '-') << '\n';
    for (size_t i = 0; i < d.runners.size(); ++i) {
      os << setw(30) << left << d.runners[i].name << right << setw(25)
         << d.runners[i].max << setw(25) << d.runners[i].mean << setw(25)
         << d.runners[i].stddev << '\n';
    }
    os << string(105, '-') << '\n';
    return os;
  }
};

int main(int argc, char** argv) {
  benchmark driver{};

  if (argc == 3) {
    stringstream input{argv[1]};
    input >> driver.n;
    input = stringstream{argv[2]};
    input >> driver.run_count;
  }

  driver  //
      .ADD(pxart::mt19937)
  // .ADD(std::mt19937)
#ifndef PXART_BENCHMARKS_DISABLE_BOOST
      .ADD(boost::random::mt19937)
#endif
      .ADD(pxart::minstd_rand)
      // .ADD(std::minstd_rand)
      .ADD(pxart::msws)
      .run();
  cout << driver;

#ifndef PXART_BENCHMARKS_DISABLE_GNUPLOT
  fstream file{"monte_carlo_pi.txt", ios::out};
  for (size_t i = 0; i < driver.runners.size(); ++i)
    file << '"' << driver.runners[i].name << '"' << '\t'
         << driver.runners[i].max << '\t' << driver.runners[i].mean << '\t'
         << driver.runners[i].stddev << '\n';
  file << flush;
  lyrahgames::gnuplot_pipe plot{};
  plot << "set xr [-1:" << driver.runners.size() << "]\n"
       << "set title \"Monte Carlo π Benchmark (samples/run = " << driver.n
       << ", runs = " << driver.run_count << ")\"\n"
       << "unset key\n"
          "set yr [0:100 < *]\n"
          "set yl \"Performance [MSamples/s]\"\n"
          "set xtics rotate by 20 right\n"
          "set xtics nomirror scale 0\n"
          "plot 'monte_carlo_pi.txt' u 0:2:(0.7):xtic(1) w boxes lc rgb "
          "'#222222' "
          "fs solid 0.2, "
          "'' u 0:3:(0.7):xtic(1) w boxes lc rgb '#222222', "
          "'' u 0:3:4 w yerrorbars lw 2 lc rgb '#222222'\n";
#endif  // PXART_BENCHMARKS_DISABLE_GNUPLOT
}