#pragma once
#include <algorithm>
#include <any>
#include <boost/type_index.hpp>
#include <cstdint>

namespace testu01_utils {

void set_rng_object(const char* name, uint32_t(oracle)());

// template <typename RNG>
// RNG engine;
inline std::any engine;

template <typename RNG>
void set_rng(const char* name, RNG&& rng) {
  using rng_type = std::decay_t<RNG>;
  engine = std::forward<RNG>(rng);
  set_rng_object(
      name, []() -> uint32_t { return std::any_cast<rng_type&>(engine)(); });
}

template <typename RNG>
void set_rng(RNG&& rng) {
  auto name = boost::typeindex::type_id<RNG>().pretty_name();
  std::replace(name.begin(), name.end(), ':', '_');
  set_rng(name.c_str(), std::forward<RNG>(rng));
}

void verbose(bool = true);
void run_battery_small_crush();
void run_battery_crush();
void run_battery_big_crush();
void run_test_linear_complexity();

}  // namespace testu01_utils