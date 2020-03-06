#include <boost/program_options.hpp>
#include <pxrun/matrix.hpp>

namespace pxrun {

matrix::matrix(const std::vector<std::string>& args) {
  using namespace std;
  using namespace boost::program_options;

  options_description parameters{"Options for command 'matrix'"};
  parameters.add_options()                           //
      ("rows", value(&rows), "Set number of rows.")  //
      ("cols", value(&cols), "Set number of cols.");

  variables_map values;
  store(                         //
      command_line_parser{args}  //
          .options(parameters)
          .positional(positional_options_description{})  // This will forbid
                                                         // positional options.
          .run(),
      values);
  notify(values);
}

}  // namespace pxrun