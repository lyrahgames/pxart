#include <cassert>
#include <iostream>
#include <pxart/mt19937.hpp>
#include <pxart/version.hpp>
#include <random>

int main(int argc, char** argv) {
  std::cout << "PXART_VERSION = " << PXART_VERSION << "\n"
            << "PXART_VERSION_STR = " << PXART_VERSION_STR << "\n"
            << "PXART_VERSION_ID = " << PXART_VERSION_ID << "\n"
            << "PXART_VERSION_MAJOR = " << PXART_VERSION_MAJOR << "\n"
            << "PXART_VERSION_MINOR = " << PXART_VERSION_MINOR << "\n"
            << "PXART_VERSION_PATCH = " << PXART_VERSION_PATCH << "\n"
            << "PXART_PRE_RELEASE = " << PXART_PRE_RELEASE << "\n"
            << "PXART_SNAPSHOT_SN = " << PXART_SNAPSHOT_SN << "\n"
            << "PXART_SNAPSHOT_ID = " << PXART_SNAPSHOT_ID << "\n";

  std::mt19937 rng{};
  pxart::mt19937 myrng{};

  const int n = 10'000'000;
  for (int i = 0; i < n; ++i) {
    const auto rnd = rng();
    const auto myrnd = myrng();
    assert(rnd == myrnd);
  }

#ifdef __AVX2__
  std::cout << "Configuration supports __AVX2__.\n";
#endif

#ifdef __SSE4_1__
  std::cout << "Configuration supports __SSE4_1__.\n";
#endif
}
