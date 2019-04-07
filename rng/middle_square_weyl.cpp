#include <fmt/format.h>

#include <librng/middle_square_weyl_engine.hpp>
#include <librng/test/histogram.hpp>
#include <librng/test/monty_hall_dilemma.hpp>

using namespace std;
using namespace std::string_literals;
using namespace fmt;

int main(int argc, char** argv) {
  using rng_type = rng::middle_square_weyl_engine;

  const auto usage = format("usage: {} [<sample size> [<bin count>]]", argv[0]);

  int sample_size = 1000;
  int bin_count = 15;

  if (argc > 1) {
    if ("help"s == argv[1]) {
      print(usage);
      return 0;
    }

    sample_size = stoi(argv[1]);
    if (argc > 2) {
      bin_count = stoi(argv[2]);
      if (argc > 3) {
        print("Too many arguments!\n{}\n", usage);
        return -1;
      }
    }
  }

  print(
      "sample size = {}\n"
      "bin count = {}\n",
      sample_size, bin_count);

  rng_type rng{};
  rng::test::histogram histogram{rng, sample_size, bin_count};
  cout << histogram << '\n';

  rng = rng_type{};
  const auto [initial, second] =
      rng::test::monty_hall_dilemma(rng, sample_size);
  print(
      "monty hall dilemma test with {} samples:\n"
      "wins on initial guess: {} ~ {}\n"
      "wins on second guess:  {} ~ {}\n",
      sample_size, initial, static_cast<double>(initial) / sample_size, second,
      static_cast<double>(second) / sample_size);
}