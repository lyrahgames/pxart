#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
//
#include <pxart/distribution/poisson.hpp>
#include <pxart/generator/mt19937.hpp>
//
#ifndef PXART_EXAMPLES_DISABLE_GNUPLOT
#include <lyrahgames/gnuplot_pipe.hpp>
#endif

using namespace std;

int main() {
  // Define parameter.
  using real = double;
  const real lambda = 15;
  const size_t samples = 10'000'000;

  // Initialize PRNG.
  pxart::mt19937 rng{std::random_device{}};
  // Initialize Poisson distribution properties class
  // for testing and comparison.
  pxart::poisson_distribution distribution{lambda};

  // Compute mean, variance, standard deviation,
  // and histogram out of Poisson-distributed samples.
  size_t sum = 0;
  size_t sum2 = 0;
  map<size_t, size_t> histogram{};
  for (size_t i = 0; i < samples; ++i) {
    const auto rnd = pxart::poisson<size_t>(rng, lambda);
    sum += rnd;
    sum2 += rnd * rnd;
    ++histogram[rnd];
  }
  const auto mean = real(sum) / real(samples);
  const auto var = (real(sum2) - samples * mean * mean) / real(samples - 1);
  const auto stddev = sqrt(var);

  // For reference and comparison, print expected and estimated values
  // for mean, variance, and standard deviation on the screen.
  // The expected values are given by the distribution properties class.
  cout << "Example: Sampling the Poisson Distribution\n"
       << setw(25) << "expected mean = " << setw(15) << distribution.mean()
       << setw(25) << "estimated mean = " << setw(15) << mean << '\n'
       << setw(25) << "expected variance = " << setw(15)
       << distribution.variance() << setw(25)
       << "estimated variance = " << setw(15) << var << '\n'
       << setw(25) << "expected stddev = " << setw(15) << distribution.sigma()
       << setw(25) << "estimated stddev = " << setw(15) << stddev << '\n';

#ifndef PXART_EXAMPLES_DISABLE_GNUPLOT
  // Use gnuplot to plot the histogram by first writing it to a file.
  // For reference and comparison, add the theoretical probability for all
  // possible values of the Poisson distribution which is again given by the
  // distribution properties class.
  fstream file{"histogram.txt", ios::out};
  for (auto [i, f] : histogram)
    file << i << ' ' << f / real(samples) << ' ' << distribution.probability(i)
         << '\n';
  file << flush;
  lyrahgames::gnuplot_pipe plot{};
  plot << "set title 'Poisson Distribution lambda = " << lambda << "'\n"
       << "plot 'histogram.txt' u 1:2 w boxes lc rgb '#222222' fs solid 0.2 "
          "title 'Relative Frequencies', "
          "'' u 1:3 w lines lc rgb '#3366ff' lw 2 title 'Ideal Probability'\n";
#endif  // PXART_EXAMPLES_DISABLE_GNUPLOT
}