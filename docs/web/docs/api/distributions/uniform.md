# Uniform Distribution
## Scalar
```c++
template <typename T, typename RNG>
inline T pxart::uniform(RNG&& rng);
```
This functions advances the state of the given random number generator to get a number and transforms it such that a sequence of returned numbers is uniformly distributed in a default type-dependent range.
The default range for floating-point types is $[0,1)$.
The default range for integral types is every possible integral number that can be stored by the given type.
---

```c++
template <typename T, typename RNG>
inline T pxart::uniform(RNG&& rng, T a, T b) noexcept;
```
This functions advances the state of the given random number generator to get a number and transforms it such that a sequence of returned numbers is uniformly distributed in the given range.
The range for floating-point types is interpreted as $[a,b)$.
The range for integral types is interpreted as $\{x\in\mathbb{Z} \ | \ a\leq x \leq b\}$.

### Floating-Point Numbers
The `#!c++ pxart::uniform` methods for floating-point types can only be used for `#!c++ float` and `#!c++ double`.

#### Notes and Implementation Details
<figure>
    <img src="../../../images/uniform_implementation_scheme.png" />
    <figcaption>Scheme of the Uniform Distribution Implementation for Floating-Point Numbers</figcaption>
</figure>

#### Example
```c++
{!../../examples/basics/uniform/scalar/float/main.cpp!}
```

### Integral Numbers
#### Notes and Implementation Details
To be fast, the `pxart::uniform` methods for integral numbers exhibit a small bias which is caused by round-off errors when truncating the shifted multiplication of two integral numbers.

#### Example
```c++
{!../../examples/basics/uniform/scalar/int/main.cpp!}
```
