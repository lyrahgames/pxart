# PCG32

## Scalar
```c++
namespace pxart {

struct pcg32;

}
```

### Include Scheme
```c++
#include <pxart/pcg32.hpp>
```

### Member Types
```c++
using uint_type = uint64_t;
using result_type = uint32_t;
```

### Member Functions
#### Construction and Seeding
```c++
constexpr pcg32();
```
Default constructor.
---

```c++
template <typename RNG>
constexpr explicit pcg32(RNG&& rng);
```
Initialize the PRNG by any other seeder or RNG.

#### Generation
```c++
constexpr result_type operator()() noexcept;
```
Return pseudorandom numbers and advance the state of the generator.

### Example
```c++
{!../../examples/basics/pcg32/scalar/main.cpp!}
```