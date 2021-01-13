# Generate

```c++
template <typename RNG, typename Iterator>
constexpr auto pxart::generate(RNG&& rng, Iterator first, Iterator last);
```

For all iterated elements `rng()` is called and used as new value.
This function can specialized for other types to provide more efficient implementations.

## Include Scheme
```c++
#include <pxart/algorithm.hpp>
```

## Example
```c++
{!../../examples/basics/generate/main.cpp!}
```