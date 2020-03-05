#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

extern "C" {
#include <TestU01.h>
}
#include <cstdint>
#include <iostream>
#include <pxart/pxart.hpp>
#include <random>
#include <sstream>

PXART_TESTU01_RNG rng{std::random_device{}};
decltype(rng()) cache;
constexpr int cache_size = sizeof(decltype(rng())) / sizeof(uint32_t);
int cache_index = 0;

inline uint32_t serializer() noexcept {
  if (!cache_index) cache = rng();
  const auto result = reinterpret_cast<const uint32_t*>(&cache)[cache_index];
  cache_index = (cache_index + 1) % cache_size;
  return result;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " <n:{0,1,2}>\n"
              << "0\tsmall crush\n"
              << "1\tcrush\n"
              << "2\tbig crush\n";
    return -1;
  }

  std::stringstream input{argv[1]};
  int n;
  input >> n;

  swrite_Basic = false;
  auto gen =
      unif01_CreateExternGenBits(STRINGIFY(PXART_TESTU01_RNG), serializer);
  switch (n) {
    case 0:
      bbattery_SmallCrush(gen);
      break;
    case 1:
      bbattery_Crush(gen);
      break;
    case 2:
      bbattery_BigCrush(gen);
      break;
  }
  unif01_DeleteExternGenBits(gen);
}