---
title: Quickstart
---

To use pXart, you have to make two decisions.
1. Which build system will be used to make pXart available to your current project?
2. Which build system will be used for your own project?

We explicitly support the installation of pXart by using build2 or CMake.
But because it is a header-only library, other build systems should work as well.

### pXart's Hello-World Example: Monte Carlo π

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