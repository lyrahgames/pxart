#include <cstdint>
#include <iostream>
#include <pxart/mt19937.hpp>
#include <pxart/mt19937_simd128.hpp>
#include <random>
#include <sstream>
#include <string>

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
  seeder seed{rd};
  pxart::mt19937 rng{seed};
  seed.reset();
  pxart::mt19937_simd128 vrng{seed};

  bool not_equal = false;

  for (int i = 0; i < n; i += 8) {
    const auto v = vrng();
    for (int j = 0; j < 4; ++j) {
      const auto s = rng();
      const auto sv = (reinterpret_cast<const uint32_t*>(&v))[j];
      const auto tmp = (sv - s != 0);
      if (tmp) not_equal = true;
      if (tmp || print) {
        std::cout << i + j << ":\t" << s << "\t-\t" << sv << "\t=\t" << s - sv
                  << "\n";
      }
    }
  }

  if (not_equal) {
    std::cerr << "Error: Not the same output.\n";
    return -1;
  } else
    std::cout << "Success: Same output.\n";
}