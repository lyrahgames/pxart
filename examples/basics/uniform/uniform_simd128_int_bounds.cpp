#include <array>
#include <iomanip>
#include <iostream>
#include <pxart/pxart.hpp>

int main() {
  using namespace std;

#ifdef __SSE4_1__

  pxart::simd128::mt19937 rng{random_device{}};
  for (int i = 0; i < 20; ++i) {
    using type = int;
    // const auto vrnd = pxart::simd128::uniform<type>(rng, -9, 9);
    const auto vrnd = pxart::uniform<type>(rng, '0', '9');
    const auto size = sizeof(vrnd) / sizeof(type);
    const auto srnd = pxart::pun_cast<array<type, size>>(vrnd);
    for (int j = 0; j < size; ++j) {
      cout << fixed << setprecision(6) << setw(4) << srnd[j];
    }
    cout << '\n';
  }

#else

  cout << "Example can only be used with SSE4.1 instruction set.\n";

#endif  // __SSE4_1__
}