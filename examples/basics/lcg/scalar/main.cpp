#include <iomanip>
#include <iostream>
#include <random>
//
#include <pxart/lcg.hpp>
#include <pxart/uniform.hpp>

using namespace std;

int main() {
  // Properly initialize pxart PRNG.
  pxart::minstd_rand rng{std::random_device{}()};
  // Print some uniformly distributed random numbers.
  for (size_t i = 0; i < 10; ++i)
    cout << setw(20) << pxart::uniform<float>(rng) << '\n';
}