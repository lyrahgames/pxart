#include <iostream>
#include <random>
#include <string>
#include <vector>

#include <boost/program_options.hpp>
#include <boost/random/mersenne_twister.hpp>

using namespace std;
using namespace boost::program_options;

struct random_matrix_printer {
  template <typename RNG>
  void print(RNG&& state) {
    RNG rng = std::forward<RNG>(state);
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) cout << rng() << "\t";
      cout << "\n";
    }
  }

  int rows = 10;
  int cols = 1;
};

int main(int argc, char* argv[]) {
  string rng_name;
  random_matrix_printer printer;

  options_description description{"My tool usage!"};
  description.add_options()                                                 //
      ("help,h", "Display this message.")                                   //
      ("verbose,v", "Make messages verbose.")                               //
      ("bit-reversal,r", "Apply bit reversal on 32 bit values.")            //
      ("rows", value<int>(&printer.rows), "Set number of rows to output.")  //
      ("cols", value<int>(&printer.cols), "Set number of cols to output.")  //
      ("rng", value<string>(&rng_name), "Set random number generator.");
  variables_map vm;
  store(command_line_parser(argc, argv).options(description).run(), vm);
  notify(vm);

  if (vm.count("help")) {
    cout << description;
    return 0;
  }

  if (rng_name == "std::mt19937")
    printer.print(std::mt19937{});
  else if (rng_name == "boost::random::mt19937")
    printer.print(boost::random::mt19937{});
  else {
    cout << "Random Number Generator not known!\n";
    return -1;
  }
}