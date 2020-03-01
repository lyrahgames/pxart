#pragma once

#include <string>
#include <vector>

#include <testu01_utils/testu01_utils.hpp>

namespace pxrun {

class testu01 {
 public:
  explicit testu01(const std::vector<std::string>& args);
  testu01() : testu01{{}} {}

  template <typename RNG>
  void run(RNG&& rng) const {
    testu01_utils::set_rng(std::forward<RNG>(rng), reverse_bits);
    testu01_utils::verbose(verbose);
    if (small_crush) testu01_utils::run_battery_small_crush();
    if (crush) testu01_utils::run_battery_crush();
    if (big_crush) testu01_utils::run_battery_big_crush();
    if (linear_complexity) testu01_utils::run_test_linear_complexity();
  }

 private:
  bool reverse_bits;
  bool verbose;
  bool small_crush;
  bool crush;
  bool big_crush;
  bool linear_complexity;
};

}  // namespace pxrun