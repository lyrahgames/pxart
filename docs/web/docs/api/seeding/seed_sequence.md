# Seed Sequence
The goal of `seed_seq` is to generate reasonable seed data based on multiple entropy arguments or multiple fixed seeds.
It is an alternative implementation of [**M. E. O'Neill's `seed_seq` Alternative**](https://www.pcg-random.org/posts/developing-a-seed_seq-alternative.html).
Take a look at her website to find out how it is working.
`seed_seq` is a k-to-1 mapping and has good avalanche properties.
In general, it provides much better behavior than `std::seed_seq`.
We recommend to use `seed_seq` when possible.

## Scalar
```c++
namespace pxart {

template <typename T, size_t N>
struct seed_seq;

using seed_seq_4x32 = seed_seq<uint32_t, 4>;
using seed_seq_8x32 = seed_seq<uint32_t, 8>;
using seed_seq_4x64 = seed_seq<uint64_t, 4>;

}
```

### Include Scheme
```c++
#include <pxart/seed_seq.hpp>
```

### Member Types and Type Variables
```c++
using uint_type = T;
using result_type = uint_type;
static constexpr size_t state_size = N;
```

### Member Functions
#### Construction
```c++
constexpr seed_seq() noexcept;
```
Default construction of `#!c++ pxart::seed_seq`.
Calls other constructor with `#!c++ std::initializer_list` and argument `#!c++ {1, 2, 3, 4}`.
---

```c++
template <typename U>
seed_seq(std::initializer_list<U> init) noexcept;
```
Constructor for multiple entropy sources or multiple fixed seed values.
This constructor calls the constructor based on input iterators.
---

```c++
template <typename InputIt>
seed_seq(InputIt begin, InputIt end) noexcept;
```
Constructor generates the state of the object by first hashing the input values and then mixing them by applying another hash and mixing function.
If the given number of values is smaller than the actual state then the given entropy will be stretched to all state values.

#### Generation
```c++
constexpr result_type operator()() noexcept;
```
Returns a pseudorandom number based on the current state of the object.
This function is used to seed other PRNGs in a reasonable way.

### Example
```c++
{!../../examples/basics/seed_seq/scalar/main.cpp!}
```