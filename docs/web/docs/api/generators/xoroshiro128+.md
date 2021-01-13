# Xoroshiro128+

<figure>
    <img src="../../../images/xrsr128p_scheme.png" />
    <figcaption>Abstract Scheme of the Xoroshiro128+</figcaption>
</figure>

## Scalar
```c++
namespace pxart {

struct xoroshiro128plus;

using xrsr128p = xoroshiro128plus;

}
```

### Include Scheme
```c++
#include <pxart/xoroshiro128plus.hpp>
```

### Member Types
```c++
using uint_type = uint64_t;
using result_type = uint_type;
```

### Member Functions
#### Construction and Seeding
```c++
xoroshiro128plus();
```
Default constructor.
---

```c++
xoroshiro128plus(uint_type x, uint_type y);
```
Parameter constructor.
---

```c++
template <typename RNG>
constexpr explicit xoroshiro128plus(RNG&& rng)
```
Initialize the PRNG by any other seeder or RNG.

#### Generation
```c++
constexpr auto operator()() noexcept;
```
Generate next pseudorandom number and advance inner state of the PRNG.
---

```c++
constexpr void jump() noexcept;
```
Advance the inner state by $2^{64}$ elements.
---

```c++
constexpr void long_jump() noexcept;
```
Advance the inner state by $2^{96}$ elements.

#### Characteristics
```c++
static constexpr auto min() noexcept { return uint_type{}; }
static constexpr auto max() noexcept { return ~uint_type{}; }
```
Return the output range of pseudorandom numbers.

### Example
```c++
{!../../examples/basics/xoroshiro128plus/scalar/main.cpp!}
```
