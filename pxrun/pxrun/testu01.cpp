#include <boost/program_options.hpp>
#include <pxrun/testu01.hpp>

namespace pxrun {

testu01::testu01(const std::vector<std::string>& args) {
  using namespace boost::program_options;

  options_description description{"Options pxrun-testu01"};
  description.add_options()                                           //
      ("verbose,v", bool_switch(&verbose), "Make messages verbose.")  //
      ("small-crush,s", bool_switch(&small_crush),
       "Run test battery 'Small Crush'.")                            //
      ("crush,m", bool_switch(&crush), "Run test battery 'Crush'.")  //
      ("big-crush,b", bool_switch(&big_crush),
       "Run test battery 'Big Crush'.")  //
      ("linear-complexity,l", bool_switch(&linear_complexity),
       "Run test 'Linear Complexity'.")  //
      ("bit-reversal,r", bool_switch(&reverse_bits),
       "Apply bit reversal on 32 bit values.");
  variables_map vm;
  store(
      command_line_parser{args}  //
          .options(description)
          .positional(positional_options_description{})  // This will forbid
                                                         // positional options.
          .run(),
      vm);
  notify(vm);
}

}  // namespace pxrun