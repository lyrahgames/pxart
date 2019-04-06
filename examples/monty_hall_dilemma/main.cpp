#include <iostream>
#include <random>

using namespace std;

int main() {
  random_device rd{};
  mt19937 rng{rd()};
  uniform_int_distribution<> distribution{0, 3};

  cout << "random_device::entropy() = " << rd.entropy() << '\n';
}