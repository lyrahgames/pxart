---
title: Quickstart
---

### pXart's Hello-World Example: Monte Carlo π

```c++
#include <pxart/pxart.hpp>
#include <iostream>

int main(){
    constexpr int samples = 100'000'000;
    const int result = 0;
    pxart::mt19937 rng{};
    for (auto i = samples; i > 0; --i){
        const auto x = pxart::uniform<float>(rng);
        const auto y = pxart::uniform<float>(rng);
        result += (x*x + y*y <= 1)?(1):(0);
    }
    std::cout << "pi = " << 4.0f * result / samples << "\n";
}
```