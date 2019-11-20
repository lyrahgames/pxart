#include <cstdint>
#include <iostream>
#include <pxart/mt19937.hpp>
#include <random>
#include <sstream>
#include <string>

#include "mt19937_simd256.hpp"

struct seeder {
  template <typename RNG>
  explicit seeder(RNG& rng) {
    for (int i = 0; i < 624; ++i) state[i] = rng();
  }

  uint32_t operator()() noexcept {
    index = (index + 1) % 624;
    return state[index];
  }

  void reset() noexcept { index = 623; }

  int index{623};
  uint32_t state[624];
};

int main(int argc, char** argv) {
  if (3 < argc || argc < 2) {
    std::cout << "usage: " << argv[0]
              << " <number of samples> [-o:print numbers]\n";
    return -1;
  }

  std::stringstream input{argv[1]};
  int n;
  input >> n;
  bool print = (argc == 3 && (std::string{argv[2]} == "-o"));

  std::random_device rd{};
  std::mt19937 rng{};
  pxart::mt19937 myrng{};

  bool not_equal = false;

  for (int i = 0; i < n; ++i) {
    const auto rnd = rng();
    const auto myrnd = myrng();
    const auto tmp = (rnd - myrnd != 0);
    if (tmp) not_equal = true;
    if (tmp || print) {
      std::cout << i << ":\t" << rnd << "\t-\t" << myrnd << "\t=\t"
                << rnd - myrnd << "\n";
    }
  }

  if (not_equal) {
    std::cerr << "Error: Not the same output.\n";
    return -1;
  } else
    std::cout << "Success: Same output.\n";
}