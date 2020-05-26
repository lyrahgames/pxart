#include <iostream>
#include <photons/application.hpp>

int main(int argc, char* argv[]) {
  using namespace std;
  photons::application app{};
  app.execute();
}
