# Type Punning with Pun Cast

```c++
#include <pxart/utility/pun_cast.hpp>
//
template <typename T, typename U>
T pxart::pun_cast(U x);
```

Reinterprets object of type `U` as object of type `T` with the same bit representation.
`T` and `U` need to have the same size.
This function adheres to the strict-aliasing rules.

Typically, such type punning is done to reinterpret floating-point types as unsigned integers to manipulate their sign bit and compute the absolute value for example.
Here, `pun_cast` can also be used to reinterpret an SIMD type as an arbitray vector type to be able to access single elements.

**Parameters**
Value of type to be punned.

**Return Value**
Type-punned value.

## Complexity
Constant time complexity.
The function should result in only one processor instruction but uses `#!c++ std::memcpy` to not violate the strict-aliasing rules.

## Exceptions
The function itself throws no exceptions.
But the inner call to `std::memcpy` could throw.

## Example

```c++
#include <iostream>
#include <iomanip>
#include <array>
#include <cstdint>
//
#include <pxart/simd256/mt19937.hpp>
#include <pxart/utility/pun_cast.hpp>

using namespace std;

int main() {
  std::mt19937 rng{};
  pxart::simd256::mt19937 vrng{};

  const int n = 10;
  const size_t simd_size = 8;
  for (int i = 0; i < n; i += simd_size) {
    const auto v256 = pxart::pun_cast<array<uint32_t, simd_size>>(vrng());
    for (int j = 0; j < simd_size; ++j) {
      const auto rnd = rng();
      const auto vrnd = v256[j];
      cout << setw(20) << rnd << setw(20) << vrnd << '\n';
    }
  }
}
```
