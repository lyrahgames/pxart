#include <iostream>
#include <random>
#include <string>
#include <variant>
#include <vector>

#include <boost/program_options.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/type_index.hpp>

using namespace std;
using namespace boost::program_options;

struct random_matrix_printer {
  template <typename RNG>
  void print(RNG&& state) const {
    cout << "# RNG type: " << boost::typeindex::type_id<RNG>().pretty_name()
         << "\n";
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

  std::variant<                    //
      std::mt19937,                //
      std::minstd_rand,            //
      std::minstd_rand0,           //
      std::random_device,          //
      boost::random::mt19937,      //
      boost::random::mt11213b,     //
      boost::random::minstd_rand,  //
      boost::random::minstd_rand0>
      rng;

  if (rng_name == "std::mt19937")
    rng = std::mt19937{};
  else if (rng_name == "std::minstd_rand")
    rng = std::minstd_rand{};
  else if (rng_name == "std::minstd_rand0")
    rng = std::minstd_rand0{};
  else if (rng_name == "std::random_device")
    rng.emplace<std::random_device>();
  else if (rng_name == "boost::random::mt19937")
    rng = boost::random::mt19937{};
  else if (rng_name == "boost::random::mt11213b")
    rng = boost::random::mt11213b{};
  else if (rng_name == "boost::random::minstd_rand")
    rng = boost::random::minstd_rand{};
  else if (rng_name == "boost::random::minstd_rand0")
    rng = boost::random::minstd_rand0{};
  else {
    cout << "Random Number Generator not known!\n";
    return -1;
  }

  // auto rng =
  //     parsed_rng_type({std::mt19937{}, "std::19937"},
  //                     {std::minstd_rand{}, "std::minstd_rand"}, rng_name);

  visit([printer](auto&& x) { printer.print(x); }, rng);
}