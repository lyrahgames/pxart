# Examples
## Scalar Monte Carlo π
```c++
#include <iostream>
//
#include <pxart/pxart.hpp>

int main(){
    constexpr int samples = 100'000'000;
    int result = 0;
    pxart::mt19937 rng{};
    for (auto i = samples; i > 0; --i){
        const auto x = pxart::uniform<float>(rng);
        const auto y = pxart::uniform<float>(rng);
        result += (x*x + y*y <= 1);
    }
    std::cout << "pi = " << 4.0f * result / samples << "\n";
}

```
## AVX2 Monte Carlo π
The following code has to be compiled with AVX2 support.
For GCC and Clang, the easiest method may be to use compilation flags `-O3` and `-march=native`.

```c++
#include <iostream>
//
#include <pxart/pxart.hpp>

int main() {
  const int samples = 10'000'000;
  pxart::simd256::mt19937 rng{};

  // Initialize vectorized buffer to count the samples that lie inside the
  // circle for every component.
  auto samples_in_circle = _mm256_setzero_si256();
  // Iterate over all samples by using the length of the SIMD register.
  for (auto i = samples; i > 0; i -= 8) {
    // Generate vectorized samples by using pxart.
    const auto x = pxart::simd256::uniform<float>(rng);
    const auto y = pxart::simd256::uniform<float>(rng);
    // Test if samples lie inside the circle.
    const auto radius = _mm256_add_ps(_mm256_mul_ps(x, x), _mm256_mul_ps(y, y));
    const auto mask = _mm256_castps_si256(
        _mm256_cmp_ps(radius, _mm256_set1_ps(1.0f), _CMP_LE_OQ));
    // Add mask to the samples inside the circle for every component.
    samples_in_circle = _mm256_add_epi32(
        samples_in_circle, _mm256_and_si256(_mm256_set1_epi32(1), mask));
  }
  // Sum up all components and scale to estimate pi.
  samples_in_circle = _mm256_hadd_epi32(samples_in_circle, samples_in_circle);
  samples_in_circle = _mm256_hadd_epi32(samples_in_circle, samples_in_circle);
  const auto pi = 4.0f *
                  (reinterpret_cast<uint32_t*>(&samples_in_circle)[0] +
                   reinterpret_cast<uint32_t*>(&samples_in_circle)[4]) /
                  samples;

  std::cout << "pi = " << pi << '\n';
}
```