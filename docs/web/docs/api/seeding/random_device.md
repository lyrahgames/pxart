# Random Device
The C++ STL provides [`std::random_device`](https://en.cppreference.com/w/cpp/numeric/random/random_device) as a default seeding structure.
For a lot of platforms, [`std::random_device`](https://en.cppreference.com/w/cpp/numeric/random/random_device) returns truly random numbers and should be used for proper seeding of other PRNGs.
Some are only providing deterministic behavior.
So an alternative seeding strategy should be used for such platforms.

## Example
```c++
{!../../examples/basics/random_device/main.cpp!}
```