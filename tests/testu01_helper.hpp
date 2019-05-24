#pragma once

#include <algorithm>
#include <boost/type_index.hpp>
#include <cstdint>

extern "C" {
#include <TestU01.h>
}

template <typename RNG>
class testu01_helper {
 public:
  explicit testu01_helper(const RNG& rng) {
    engine = rng;
    auto name = boost::typeindex::type_id<RNG>().pretty_name();
    std::replace(name.begin(), name.end(), ':', '_');
    gen_ = unif01_CreateExternGenBits(const_cast<char*>(name.c_str()),
                                      []() -> uint32_t { return engine(); });
  }
  ~testu01_helper() { unif01_DeleteExternGenBits(gen_); }

  void battery_small_crush() { bbattery_SmallCrush(gen_); }
  void battery_crush() { bbattery_Crush(gen_); }
  void battery_big_crush() { bbattery_BigCrush(gen_); }

 private:
  static RNG engine;
  unif01_Gen* gen_;
};

template <typename RNG>
RNG testu01_helper<RNG>::engine{};