#include <pxart/middle_square_weyl_engine.hpp>
#include <random>
#include "testu01_helper.hpp"

int main(int argc, char* argv[]) {
  // swrite_Basic = false;
  {
    testu01_helper test{pxart::middle_square_weyl_engine{}};
    test.battery_small_crush();
    // test.battery_crush();
  }
  {
    testu01_helper test2{std::mt19937{}};
    test2.battery_small_crush();
  }
}