#include <array>
#include <cstdint>
#include <iostream>
#include <pxart/xoroshiro128plus.hpp>
#include <pxart/xoroshiro128plus_simd256.hpp>
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
  void set(int i) noexcept { index = (i - 1 + 624) % 624; }

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
  pxart::xs128p_simd256 vrng{seed};
  pxart::xs128p rng[4]{
      {seed.state[0], seed.state[1], seed.state[8], seed.state[9]},
      {seed.state[2], seed.state[3], seed.state[10], seed.state[11]},
      {seed.state[4], seed.state[5], seed.state[12], seed.state[13]},
      {seed.state[6], seed.state[7], seed.state[14], seed.state[15]}};

  bool not_equal = false;

  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 4; ++j) {
      const auto s = rng[j].s[i];
      const auto sv = (reinterpret_cast<const uint64_t*>(&(vrng.state[i])))[j];
      std::cout << s << "\t" << sv << "\n";
    }
  }

  for (int i = 0; i < n; i += 8) {
    const auto v = vrng();
    for (int j = 0; j < 4; ++j) {
      const auto s = rng[j]();
      const auto sv = (reinterpret_cast<const uint64_t*>(&v))[j];
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