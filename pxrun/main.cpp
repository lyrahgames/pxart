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

using namespace std;
namespace po = boost::program_options;

int main(int argc, char* argv[]) {
  // Set global program options.
  bool help;
  boost::optional<string> rng_type;
  boost::optional<string> cmd;
  vector<string> cmd_args;
  po::options_description global_options{"Global Options"};
  global_options.add_options()                                     //
      ("help,h", po::bool_switch(&help), "Display this message.")  //
      ("rng", po::value(&rng_type),
       "Set random number generator to be used for task.")  //
      ("cmd", po ::value(&cmd), "Command")                  //
      ("cmd_args", po::value(&cmd_args), "...");
  // Create positional options to use commands with command-specific options.
  po::positional_options_description global_positional;
  global_positional  //
      .add("rng", 1)
      .add("cmd", 1)
      .add("cmd_args", -1);
  // Parse command line and put values in variables map.
  auto parsed_values = po::command_line_parser{argc, argv}  //
                           .options(global_options)
                           .positional(global_positional)
                           .allow_unregistered()
                           .run();
  po::variables_map arguments;
  po::store(parsed_values, arguments);
  po::notify(arguments);  // Important: Sets values for optionals!
  // Collect all unrecognized options.
  vector<string> additional_options =
      po::collect_unrecognized(parsed_values.options, po::include_positional);
  // additional_options.erase(additional_options.begin(),
  //                          additional_options.begin() + 2);

  if (rng_type) cout << "rng_type = " << *rng_type << "\n";
  if (cmd) cout << "cmd = " << *cmd << "\n";
  if (!cmd_args.empty()) {
    cout << "cmd_args = ";
    for (auto& x : cmd_args) cout << x << "\t";
    cout << "\n";
  }
  if (!additional_options.empty()) {
    cout << "additional_options = ";
    for (auto& x : additional_options) cout << x << "\t";
    cout << "\n";
  }

  return 0;

  // Filter out errors.
  if (help || !rng_type) {
    if (!help && !rng_type)
      cout << "Error: No random number generator was specified.\n";
    cout << "Usage:\n"
         << argv[0] << " [Global Options] [rng] [cmd] [cmd options]\n\n"
         << global_options;
    // return -(!help && !rng_type);
  }

  // Set random number generator.
  std::variant<          //
      std::mt19937,      //
      std::minstd_rand,  //
      std::minstd_rand0>
      rng;
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

  // Evaluate the command and its specific options.
  if (*cmd == "matrix") {
    // Set specific options for command 'matrix'.
    int rows = 10;
    int cols = 1;
    po::options_description matrix_options{"Options for command 'matrix'"};
    matrix_options.add_options()                           //
        ("rows", po::value(&rows), "Set number of rows.")  //
        ("cols", po::value(&cols), "Set number of cols.");
    // Parse additional arguments.
    po::variables_map matrix_arguments;
    po::store(                                       //
        po::command_line_parser{additional_options}  //
            .options(matrix_options)
            .run(),
        matrix_arguments);
    po::notify(matrix_arguments);

    if (help) cout << matrix_options;

    visit(
        [rows, cols](auto&& state) {
          using RNG = remove_reference_t<decltype(state)>;
          RNG engine = std::forward<RNG>(state);
          for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) cout << engine() << "\t";
            cout << "\n";
          }
        },
        rng);
  }
}