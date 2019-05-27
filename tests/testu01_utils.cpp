#include "testu01_utils.hpp"

#include <cstdint>
#include <memory>
#include <random>

extern "C" {
#include <TestU01.h>
}

namespace testu01_utils {

namespace {

std::random_device rd{};
std::unique_ptr<unif01_Gen, decltype(&unif01_DeleteExternGenBits)> rng_object{
    unif01_CreateExternGenBits(const_cast<char*>("std random_device"),
                               []() -> uint32_t { return rd(); }),
    &unif01_DeleteExternGenBits};

}  // namespace

void set_rng_object(const char* name, uint32_t(oracle)()) {
  rng_object.reset(nullptr);
  rng_object.reset(unif01_CreateExternGenBits(const_cast<char*>(name), oracle));
}

void verbose(bool v) { swrite_Basic = v; }
void run_battery_small_crush() { bbattery_SmallCrush(rng_object.get()); }
void run_battery_crush() { bbattery_Crush(rng_object.get()); }
void run_battery_big_crush() { bbattery_BigCrush(rng_object.get()); }
void run_test_linear_complexity() {
  scomp_Res* res = scomp_CreateRes();
  const auto tmp = swrite_Basic;
  swrite_Basic = TRUE;
  for (int size : {250, 500, 1000, 5000, 25000, 50000, 75000})
    scomp_LinearComp(rng_object.get(), res, 1, size, 0, 1);
  swrite_Basic = tmp;
  scomp_DeleteRes(res);
}

}  // namespace testu01_utils