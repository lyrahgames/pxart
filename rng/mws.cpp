extern "C" {
#include <bbattery.h>
#include <unif01.h>
}

#include "middle_square_weyl_engine.hpp"

unsigned int middle_weyl() {
  static pxart::middle_square_weyl_engine rng;
  return rng();
}

int main(int argc, char* argv[]) {
  const auto gen = unif01_CreateExternGenBits("Middle Weyl", middle_weyl);
  bbattery_SmallCrush(gen);
  unif01_DeleteExternGenBits(gen);
}