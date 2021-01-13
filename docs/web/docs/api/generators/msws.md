# MSWS

<figure>
    <img src="../../../images/msws_scheme.png" />
    <figcaption>Abstract Scheme of the MSWS</figcaption>
</figure>

## Scalar
```c++
namespace pxart {

struct msws;

}
```

### Include Scheme
```c++
#include <pxart/msws.hpp>
```

### Member Types
```c++
using uint_type = uint64_t;
using result_type = uint32_t;
```

### Member Functions
#### Construction and Seeding
```c++
constexpr msws();
```
Default constructor.
---

```c++
template <typename RNG>
explicit msws(RNG&& rng);
```
Initialize the PRNG by any other seeder or RNG.

#### Generation
```c++
constexpr result_type operator()() noexcept;
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
static constexpr result_type min() noexcept;
static constexpr result_type max() noexcept;
```
Return the output range of pseudorandom numbers.

### Example
```c++
{!../../examples/basics/msws/scalar/main.cpp!}
```
