#include <doctest/doctest.h>

#include <pxart/utilities/is_valid.hpp>

namespace {

constexpr auto has_test = pxart::is_valid(
    [](auto&& x) -> std::enable_if_t<std::is_same_v<decltype(x.test()), int>> {
    });

// default test function
template <typename T>
auto test(T&& t) -> std::enable_if_t<!decltype(has_test(t))::value, int> {
  return 0;
}
// Call member function 'test' if it exists.
template <typename T>
auto test(T&& t) -> std::enable_if_t<decltype(has_test(t)){}(), int> {
  return t.test();
}

struct A {
  int test() { return 1; }
};

struct B {};

struct C {
  void test() {}
};

}  // namespace

TEST_CASE(
    "Use pxart::is_valid for helper function specialization based on member "
    "function existence.") {
  A a{};
  B b{};
  C c{};
  CHECK(test(a) == 1);
  CHECK(test(b) == 0);
  CHECK(test(c) == 0);
}