# MT19937 Default Seeder
The purpose of `#!c++ pxart::mt19937::default_seeder` is to provide reasonable pseudorandom seeds based on a single truly or fixed seed value.
It is implemented in such a way that [`pxart::mt19937`](../generators/mt19937.md) outputs the same values in the same order as `std::mt19937` when `pxart::mt19937::default_seeder` is initialized with the same integer value than `std::mt19937`.

!!! note
    We do not recommend to use `#!c++ pxart::mt19937::default_seeder` because it only uses a single seed value.
    Hence, we designed the interface to be a little bit more complicated to emphasize the seeding mechanism.
    It is implemented for the sake of completeness.
    Instead you should use [`#!c++ pxart::seed_seq`](seed_sequence.md).

```c++
namespace pxart {

struct mt19937::default_seeder;

}
```

## Include Scheme
```c++
#include <pxart/mt19937.hpp>
```

## Member Functions
### Construction
```c++
constexpr default_seeder();
```
Default constructor uses a default seed value and the one-parameter constructor.
---

```c++
constexpr explicit default_seeder(uint_type s);
```
Initializes the object by a single given integer.

### Generation
```c++
constexpr uint_type operator()() noexcept;
```
Generates reasonable pseudorandom seed values for the MT19937 based on a single fixed value or a single truly random seed.

### Characteristics
```c++
constexpr uint_type min() noexcept;
constexpr uint_type max() noexcept;
```
These functions return the output range of the seeder.

## Example
```c++
{!../../examples/basics/mt19937/default_seeder/main.cpp!}
```
