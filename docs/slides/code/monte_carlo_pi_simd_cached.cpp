// ...
#include <pxart/pxart.hpp>
// ...
pxart::mt19937 rng{};
const int samples = 100000000;
int pi = 0;
for (auto i = samples; i > 0; --i) {
  const auto x = pxart::uniform<float>(rng);
  const auto y = pxart::uniform<float>(rng);
  pi += (x * x + y * y <= 1);
}
pi = 4.0f * pi / samples;

// ...

constexpr int cache_size = sizeof(decltype(rng1())) / sizeof(Real);
Integer samples_in_circle{};
for (Integer i = 0; i < samples; i += cache_size) {
  const auto cache_x = pxart::uniform<Real>(rng1);
  const auto cache_y = pxart::uniform<Real>(rng2);
  for (int j = 0; j < cache_size; ++j) {
    const auto x = reinterpret_cast<const Real*>(&cache_x)[j];
    const auto y = reinterpret_cast<const Real*>(&cache_y)[j];
    samples_in_circle += (x * x + y * y <= 1);
  }
}
return static_cast<Real>(samples_in_circle) / samples * 4;