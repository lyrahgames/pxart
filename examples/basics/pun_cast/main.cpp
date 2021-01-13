#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
//
#include <pxart/simd256/mt19937.hpp>
#include <pxart/utility/pun_cast.hpp>

using namespace std;

int main() {
  // Define standard random number generator for comparisons.
  std::mt19937 rng{};

  // Define PRNG which uses 256-bit vector registers as output.
  pxart::simd256::mt19937 vrng{};
  using uint_type = decltype(vrng)::uint_type;
  constexpr size_t simd_size = decltype(vrng)::simd_size;

  constexpr size_t n = 10;
  for (int i = 0; i < n; i += simd_size) {
    // Generate random 256-bit vector and reinterpret it as 8-dimensional array
    // of 32-bit unsigned integers.
    const auto vrnd = pxart::pun_cast<array<uint_type, simd_size>>(vrng());

    // Output generated random numbers.
    for (int j = 0; j < simd_size; ++j) {
      const auto rnd = rng();
      cout << setw(20) << rnd << setw(20) << vrnd[j] << '\n';
    }
  }
}