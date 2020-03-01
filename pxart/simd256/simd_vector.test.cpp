#include <doctest/doctest.h>

#include <pxart/simd256/simd_vector.hpp>
#include <random>

using namespace std;

struct A {
  int a;
};
struct B : A {};
struct C : B {};

A operator+(A x, A y) { return {x.a + y.a}; }
B operator-(B x, B y) { return {x.a - y.a}; }

TEST_CASE("") {
  C c1, c2;
  c1 - c2;
  c1 + c2 - c2;
}