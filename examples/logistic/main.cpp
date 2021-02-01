#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
//
#include <pxart/distribution/logistic.hpp>
#include <pxart/generator/mt19937.hpp>
//
#ifndef PXART_EXAMPLES_DISABLE_GNUPLOT
#include <lyrahgames/gnuplot_pipe.hpp>
#endif

using namespace std;

int main() {
  // Define parameters for distribution and histogram.
  using real = double;
  const real mu = 10;
  const real s = 20;
  const size_t samples = 10'000'000;
  const real bin_width = 0.5 * s;

  // Initialize PRNG.
  pxart::mt19937 rng{std::random_device{}};
  // Initialize logistic distribution properties class
  // for testing and comparison.
  pxart::logistic_distribution distribution{mu, s};

  // Compute mean, variance, standard deviation,
  // and histogram out of logistic-distributed samples.
  real sum = 0;
  real sum2 = 0;
  map<int, size_t> histogram{};
  for (size_t i = 0; i < samples; ++i) {
    const auto rnd = pxart::logistic(rng, mu, s);
    // const auto b = rng();
    // const auto u = pxart::detail::uniform<real>(b);
    // const auto rnd = pxart::detail::logistic(u, mu, s);
    // if (abs(rnd - mu) > 10 * distribution.sigma()) {
    //   std::cout << "exception: b = " << b << ", u = " << u << ", rnd = " <<
    //   rnd
    //             << '\n';
    //   continue;
    // }
    sum += rnd;
    sum2 += rnd * rnd;
    ++histogram[static_cast<int>(round(rnd / bin_width))];
  }
  const auto mean = sum / samples;
  const auto var = (sum2 - samples * mean * mean) / (samples - 1);
  const auto stddev = sqrt(var);

  // For reference and comparison, print expected and estimated values
  // for mean, variance, and standard deviation on the screen.
  // The expected values are given by the distribution properties class.
  cout << "Example: Sampling the Logistic Distribution\n"
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
  // possible values of the logistic distribution which is again given by the
  // distribution properties class.
  fstream file{"histogram.txt", ios::out};
  for (auto [i, f] : histogram)
    file << i * bin_width << ' ' << f / bin_width / real(samples) << '\n';
  file << flush;
  lyrahgames::gnuplot_pipe plot{};
  plot << "set title 'Logistic Distribution mu = " << mu << ", s = " << s
       << "'\n"
       << "p(x) = " << distribution.pdf_string() << '\n'
       << "set samples 1000\n"
       << "plot 'histogram.txt' u 1:2 w boxes lc rgb '#222222' fs solid 0.2 "
          "title 'Relative Frequencies', "
          "p(x) w lines lc rgb '#3366ff' lw 2 title 'Ideal Probability "
          "Density'\n";
#endif  // PXART_EXAMPLES_DISABLE_GNUPLOT
}