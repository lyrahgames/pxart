extern "C" {
#include <TestU01.h>
}

#include <pxart/xoroshiro128plus.hpp>
#include <random>

int main(int argc, char* argv[]) {
  auto gen =
      unif01_CreateExternGenBits("Xoroshiro128+ v2018", []() -> unsigned int {
        static std::random_device rd{};
        static rng::xs128p engine{rd(), rd()};
        return engine();
      });
  bbattery_SmallCrush(gen);
  unif01_DeleteExternGenBits(gen);
}