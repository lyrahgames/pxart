# Seed Sequence

## Scalar
```c++
#include <pxart/mt19937.hpp>
// ...
namespace pxart {
template <typename T, size_t N>
struct seed_seq;
}
//
using seed_seq_4x32 = seed_seq<uint32_t, 4>;
using seed_seq_8x32 = seed_seq<uint32_t, 8>;
using seed_seq_4x64 = seed_seq<uint64_t, 4>;
```

### Member Types
```c++
using uint_type = T;
using result_type = uint_type;
static constexpr size_t state_size = N;
```

### Member Functions

#### Construction
```c++
template <typename InputIt>
seed_seq(InputIt begin, InputIt end) noexcept;
template <typename U>
seed_seq(std::initializer_list<U> init) noexcept;
constexpr seed_seq() noexcept;
```
#### Generation
```c++
constexpr result_type operator()() noexcept;
```

### Notes
The original concept has been developed by Melissa O'Neill.

### Example
```c++
{!../../examples/basics/seed_seq/main.cpp!}
```