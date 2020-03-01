#include <iostream>
#include <random>
#include <string>
#include <vector>

#include <boost/program_options.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/mersenne_twister.hpp>

#include <pxart/middle_square_weyl_engine.hpp>

#include <testu01_utils/testu01_utils.hpp>

using namespace std;
using namespace boost::program_options;

int main(int argc, char* argv[]) {
  string rng_name;

  options_description description{"My tool usage!"};
  description.add_options()                                       //
      ("help,h", "Display this message.")                         //
      ("verbose,v", "Make messages verbose.")                     //
      ("small-crush,s", "Run test battery 'Small Crush'.")        //
      ("crush,m", "Run test battery 'Crush'.")                    //
      ("big-crush,b", "Run test battery 'Big Crush'.")            //
      ("linear-complexity,l", "Run test 'Linear Complexity'.")    //
      ("bit-reversal,r", "Apply bit reversal on 32 bit values.")  //
      ("rng", value<string>(&rng_name)->required(),
       "Set random number generator.");
  variables_map vm;
  store(command_line_parser(argc, argv).options(description).run(), vm);
  notify(vm);

  if (vm.count("help")) {
    cout << description;
    return 0;
  }

  // const auto rng_name = vm["rng"].as<string>();
  bool reverse_bits = vm.count("bit-reversal");

  if (rng_name == "middle-square-weyl")
    testu01_utils::set_rng(pxart::middle_square_weyl_engine{}, reverse_bits);
  else if (rng_name == "std::mt19937")
    testu01_utils::set_rng(std::mt19937{}, reverse_bits);
  else if (rng_name == "std::minstd_rand")
    testu01_utils::set_rng(std::minstd_rand{}, reverse_bits);
  else if (rng_name == "std::minstd_rand0")
    testu01_utils::set_rng(std::minstd_rand0{}, reverse_bits);
  else if (rng_name == "std::random_device")  // random_device cannot be copied
    testu01_utils::set_rng(
        rng_name,
        []() {
          static std::random_device rd{};
          return rd();
        },
        reverse_bits);
  else if (rng_name == "boost::random::mt19937")
    testu01_utils::set_rng(boost::random::mt19937{}, reverse_bits);
  else if (rng_name == "boost::random::mt11213b")
    testu01_utils::set_rng(boost::random::mt11213b{}, reverse_bits);
  else if (rng_name == "boost::random::minstd_rand")
    testu01_utils::set_rng(boost::random::minstd_rand{}, reverse_bits);
  else if (rng_name == "boost::random::minstd_rand0")
    testu01_utils::set_rng(boost::random::minstd_rand0{}, reverse_bits);
  else {
    cout << "Random Number Generator not known!\n";
    return -1;
  }

  testu01_utils::verbose(vm.count("verbose"));
  if (vm.count("small-crush")) testu01_utils::run_battery_small_crush();
  if (vm.count("crush")) testu01_utils::run_battery_crush();
  if (vm.count("big-crush")) testu01_utils::run_battery_big_crush();
  if (vm.count("linear-complexity"))
    testu01_utils::run_test_linear_complexity();
}