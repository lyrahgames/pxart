#include <iomanip>
#include <iostream>
#include <pxart/pxart.hpp>

int main() {
  using namespace std;
  pxart::mt19937 rng{random_device{}};
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 5; ++j) {
      cout << fixed << setw(15) << pxart::uniform<unsigned int>(rng);
    }
    cout << '\n';
  }
}