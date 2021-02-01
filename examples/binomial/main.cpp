#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
//
#include <pxart/distribution/binomial.hpp>
#include <pxart/generator/mt19937.hpp>
//
#ifndef PXART_EXAMPLES_DISABLE_GNUPLOT
#include <lyrahgames/gnuplot_pipe.hpp>
#endif

using namespace std;

int main() {
  // Define parameters of distribution.
  using real = double;
  const unsigned int n = 100;
  const real p = 0.7f;
  const size_t samples = 10'000'000;

  // Initialize PRNG and binomial distribution.
  pxart::mt19937 rng{std::random_device{}};
  // Use the default binomial structure to precompute
  // a look-up table of the cumulative distribution function
  // to speed up the process of drawing samples.
  pxart::binomial binomial{n, p};

  // Compute mean, variance, stdandard deviation,
  // and histogram out of binomially distributed samples.
  size_t sum = 0;
  size_t sum2 = 0;
  size_t histogram[n + 1]{};
  for (size_t i = 0; i < samples; ++i) {
    // Draw binomially distributed sample by using the
    // inversion method based on a precomputed look-up table.
    // This is the fastest method.
    const auto rnd = binomial(rng);

    // Draw binomially distributed sample by simulating
    // coin flips without precomputations.
    // This is much slower but may be more reliable
    // due to round-off errors when using a look-up table.
    // const auto rnd = pxart::simulation::binomial(rng, n, p);

    sum += rnd;
    sum2 += rnd * rnd;
    ++histogram[rnd];
  }
  const auto mean = real(sum) / real(samples);
  const auto var = (real(sum2) - samples * mean * mean) / real(samples - 1);
  const auto stddev = sqrt(var);

  // For reference and comparison, print expected and estimated values
  // for mean, variance, and standard deviation on the screen.
  cout << "Example: Sampling the Binomial Distribution\n"
       << setw(25) << "expected mean = " << setw(15) << binomial.mean()
       << setw(25) << "estimated mean = " << setw(15) << mean << '\n'
       << setw(25) << "expected variance = " << setw(15) << binomial.var()
       << setw(25) << "estimated variance = " << setw(15) << var << '\n'
       << setw(25) << "expected stddev = " << setw(15) << binomial.stddev()
       << setw(25) << "estimated stddev = " << setw(15) << stddev << '\n';

#ifndef PXART_EXAMPLES_DISABLE_GNUPLOT
  // Use gnuplot to plot the histogram by first writing it to a file.
  // For reference and comparison, add the theoretical probability for all
  // possible values of the binomial distribution.
  fstream file{"histogram.txt", ios::out};
  for (size_t i = 0; i <= n; ++i)
    file << i << ' ' << histogram[i] / real(samples) << ' '
         << binomial.probability(i) << '\n';
  file << flush;
  lyrahgames::gnuplot_pipe plot{};
  plot << "set title 'Binomial Distribution n = " << n << ", p = " << p << "'\n"
       << "plot 'histogram.txt' u 1:2 w boxes lc rgb '#222222' fs solid 0.2 "
          "title 'Relative Frequencies', "
          "'' u 1:3 w lines lc rgb '#3366ff' lw 2 title 'Ideal Probability'\n";
#endif  // PXART_EXAMPLES_DISABLE_GNUPLOT
}