# Linear Congruential Generator

## Scalar
```c++
namespace pxart {

template <typename Result_type, typename Uint_type, 
          Uint_type a, Uint_type c, Uint_type m>
struct lcg;

using minstd_rand = lcg<uint32_t, uint64_t, 48271, 0, 2147483647>;

}
```

### Include Scheme
```c++
#include <pxart/lcg.hpp>
```

### Member Types
```c++
using uint_type = Uint_type;
using result_type = Result_type;
```

### Member Functions
#### Construction and Seeding
```c++
constexpr lcg();
```
Default constructor.
---

```c++
constexpr explicit lcg(uint_type seed) noexcept;
```
Initialize the PRNG by using a single seed value.

#### Generation
```c++
constexpr result_type operator()() noexcept;
```
Return pseudorandom numbers and advance the state of the generator.

### Example
```c++
{!../../examples/basics/lcg/scalar/main.cpp!}
```