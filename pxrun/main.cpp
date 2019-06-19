#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include <boost/optional.hpp>
#include <boost/program_options.hpp>

#include <testu01_utils/testu01_utils.hpp>

#include <pxrun/matrix.hpp>

using namespace std;
namespace po = boost::program_options;

int main(int argc, char* argv[]) {
  bool help;
  boost::optional<string> rng_type;
  boost::optional<string> cmd;

  // Positional options should be not be shown in help message. Therefore they
  // are hidden.
  po::options_description hidden_parameters{};
  hidden_parameters.add_options()           //
      ("cmd", po ::value(&cmd), "Command")  //
      ("cmd_args", po::value<vector<string>>(), "Command arguments");

  // Define visible parameters to be shown in help message.
  po::options_description visible_parameters{"pXrun Options"};
  visible_parameters.add_options()                                 //
      ("help,h", po::bool_switch(&help), "Display this message.")  //
      ("rng", po::value(&rng_type),
       "Set random number generator to be used for task.");

  po::options_description all_parameters{};
  all_parameters.add(hidden_parameters).add(visible_parameters);

  // Parse command line and put values in variables map.
  auto parsed_values =                     //
      po::command_line_parser{argc, argv}  //
          .options(all_parameters)
          .positional(po::positional_options_description{}  //
                          .add("cmd", 1)
                          .add("cmd_args", -1))
          .allow_unregistered()
          .run();
  po::variables_map values;
  po::store(parsed_values, values);
  po::notify(values);  // Important: Sets values for optionals!
  // Collect all unrecognized options for command arguments.
  vector<string> cmd_arguments =
      po::collect_unrecognized(parsed_values.options, po::include_positional);
  cmd_arguments.erase(cmd_arguments.begin());  // Delete occurence of cmd.

  // if (rng_type) cout << "rng_type = " << *rng_type << "\n";
  // if (cmd) cout << "cmd = " << *cmd << "\n";
  // if (!cmd_arguments.empty()) {
  //   cout << "cmd_arguments = ";
  //   for (auto& x : cmd_arguments) cout << x << "\t";
  //   cout << "\n";
  // }

  // Filter out errors.
  if (help || !cmd) {
    if (!help && !cmd)
      cout << "Error: No random number generator was specified.\n";
    cout << "Usage:\n"
         << argv[0] << " [Global Options] [rng] [cmd] [cmd options]\n\n"
         << visible_parameters;
    return -(!help && !cmd);
  }

  // Set random number generator.
  std::variant<          //
      std::mt19937,      //
      std::minstd_rand,  //
      std::minstd_rand0>
      rng;
  if (rng_type) {
    if (*rng_type == "std::mt19937")
      rng = std::mt19937{};
    else if (*rng_type == "std::minstd_rand")
      rng = std::minstd_rand{};
    else if (*rng_type == "std::minstd_rand0")
      rng = std::minstd_rand0{};
    else {
      cout << "The given random number generator is not known!\n";
      return -1;
    }
  }

  // Run given module.
  if (*cmd == "matrix") {
    pxrun::matrix module{cmd_arguments};
    visit([&](auto&& x) { module.run(x); }, rng);
  }
  // else if (*cmd == "testu01") {
  //   pxrun::testu01 module{cmd_arguments};
  //   visit([&](auto&& x) { module.run(x); }, rng);
  // }
}