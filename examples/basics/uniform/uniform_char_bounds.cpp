#include <iomanip>
#include <iostream>
#include <pxart/pxart.hpp>

int main() {
  using namespace std;
  pxart::mt19937 rng{random_device{}};
  for (int i = 0; i < 20; ++i) {
    cout << setw(5) << pxart::uniform(rng, '0', '9') << setw(5)
         << pxart::uniform(rng, 'A', 'Z') << setw(5)
         << pxart::uniform(rng, 'a', 'z') << setw(5)
         << pxart::uniform(rng, '0', '4') << setw(5)
         << pxart::uniform(rng, '5', '9') << '\n';
  }
}