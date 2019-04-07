#include <fmt/format.h>

#include <librng/middle_square_engine.hpp>
#include <librng/test/histogram.hpp>

using namespace std;
using namespace std::string_literals;
using namespace fmt;

int main(int argc, char** argv) {
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

  rng::middle_square_engine rng{};
  rng::test::histogram histogram{rng, sample_size, bin_count};
  cout << histogram << '\n';
}