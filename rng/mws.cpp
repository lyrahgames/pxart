extern "C" {
#include <bbattery.h>
#include <unif01.h>
}

#include <librng/middle_square_weyl_engine.hpp>

unsigned int middle_weyl() {
  static rng::middle_square_weyl_engine rng;
  return rng();
}

int main(int argc, char* argv[]) {
  const auto gen = unif01_CreateExternGenBits("Middle Weyl", middle_weyl);
  bbattery_SmallCrush(gen);
  unif01_DeleteExternGenBits(gen);
}