#include <iomanip>
#include <iostream>
#include <pxart/pxart.hpp>

int main() {
  using namespace std;
  pxart::xrsr128p rng{random_device{}};
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 20; ++j) {
      cout << fixed << setw(3) << pxart::uniform(rng, 1, 6);
    }
    cout << '\n';
  }
}