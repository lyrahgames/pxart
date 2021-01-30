#pragma once
#include <cstdint>
#include <functional>
#include <type_traits>
//
#ifdef __cpp_concepts
#include <concepts>
#else
#warning "Development: Concepts are not enabled."
#endif

// Helper Macros to count the number of arguments in a variadic macro.
#define PXART_VAR_NUM_DEC \
  20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1
#define PXART_VAR_NUM_ARGS(...) \
  PXART_VAR_NUM_ARGS_TUPLE((__VA_ARGS__, PXART_VAR_NUM_DEC))
#define PXART_VAR_NUM_ARGS_TUPLE(TUPLE) PXART_VAR_NUM_ARGS_DETAIL TUPLE
#define PXART_VAR_NUM_ARGS_DETAIL(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10,     \
                                  A11, A12, A13, A14, A15, A16, A17, A18, A19, \
                                  A20, N, ...)                                 \
  N
#define PXART_VAR_GLUE_2(X, Y) PXART_VAR_GLUE_1(X, Y)
#define PXART_VAR_GLUE_1(X, Y) PXART_VAR_GLUE_(X, Y)
#define PXART_VAR_GLUE_(X, Y) X##Y

// Non-Variadic Concept Support Macros for C++14 and C++17.
#ifdef __cpp_concepts

#define PXART_TEMPLATE_UNARY(CONCEPT, TYPE) template <CONCEPT TYPE>

#define PXART_TEMPLATE_UNARY_REQUIRE(CONCEPT, TYPE, REQUIRE) \
  template <CONCEPT TYPE>                                    \
  requires REQUIRE

#define PXART_TEMPLATE_BINARY(CONCEPT1, TYPE1, CONCEPT2, TYPE2) \
  template <CONCEPT1 TYPE1, CONCEPT2 TYPE2>

#define PXART_TEMPLATE_BINARY_REQUIRE(CONCEPT1, TYPE1, CONCEPT2, TYPE2, \
                                      REQUIRE)                          \
  template <CONCEPT1 TYPE1, CONCEPT2 TYPE2>                             \
  requires REQUIRE

#define PXART_TEMPLATE_TERNARY(CONCEPT1, TYPE1, CONCEPT2, TYPE2, CONCEPT3, \
                               TYPE3)                                      \
  template <CONCEPT1 TYPE1, CONCEPT2 TYPE2, CONCEPT3 TYPE3>

#define PXART_TEMPLATE_TERNARY_REQUIRE(CONCEPT1, TYPE1, CONCEPT2, TYPE2, \
                                       CONCEPT3, TYPE3, REQUIRE)         \
  template <CONCEPT1 TYPE1, CONCEPT2 TYPE2, CONCEPT3 TYPE3>              \
  requires REQUIRE

#else  // __cpp_concepts

#define PXART_TEMPLATE_UNARY(CONCEPT, TYPE) \
  template <typename TYPE, std::enable_if_t<CONCEPT<TYPE>, int> = 0>

#define PXART_TEMPLATE_UNARY_REQUIRE(CONCEPT, TYPE, REQUIRE) \
  template <typename TYPE,                                   \
            std::enable_if_t<CONCEPT<TYPE> && (REQUIRE), int> = 0>

#define PXART_TEMPLATE_BINARY(CONCEPT1, TYPE1, CONCEPT2, TYPE2) \
  template <typename TYPE1, typename TYPE2,                     \
            std::enable_if_t<CONCEPT1<TYPE1> && CONCEPT2<TYPE2>, int> = 0>

#define PXART_TEMPLATE_BINARY_REQUIRE(CONCEPT1, TYPE1, CONCEPT2, TYPE2,       \
                                      REQUIRE)                                \
  template <typename TYPE1, typename TYPE2,                                   \
            std::enable_if_t<CONCEPT1<TYPE1> && CONCEPT2<TYPE2> && (REQUIRE), \
                             int> = 0>

#define PXART_TEMPLATE_TERNARY(CONCEPT1, TYPE1, CONCEPT2, TYPE2, CONCEPT3,    \
                               TYPE3)                                         \
  template <                                                                  \
      typename TYPE1, typename TYPE2, typename TYPE3,                         \
      std::enable_if_t<CONCEPT1<TYPE1> && CONCEPT2<TYPE2> && CONCEPT3<TYPE3>, \
                       int> = 0>

#define PXART_TEMPLATE_TERNARY_REQUIRE(CONCEPT1, TYPE1, CONCEPT2, TYPE2, \
                                       CONCEPT3, TYPE3, REQUIRE)         \
  template <typename TYPE1, typename TYPE2, typename TYPE3,              \
            std::enable_if_t<CONCEPT1<TYPE1> && CONCEPT2<TYPE2> &&       \
                                 CONCEPT3(TYPE3) && (REQUIRE),           \
                             int> = 0>
#endif  // __cpp_concepts

// Concept Support Macros for Variadic pxart Template Macro
#define PXART_TEMPLATE_2(CONCEPT, TYPE) PXART_TEMPLATE_UNARY(CONCEPT, TYPE)
#define PXART_TEMPLATE_3(CONCEPT, TYPE, REQUIRE) \
  PXART_TEMPLATE_UNARY_REQUIRE(CONCEPT, TYPE, REQUIRE)
#define PXART_TEMPLATE_4(CONCEPT1, TYPE1, CONCEPT2, TYPE2) \
  PXART_TEMPLATE_BINARY(CONCEPT1, TYPE1, CONCEPT2, TYPE2)
#define PXART_TEMPLATE_5(CONCEPT1, TYPE1, CONCEPT2, TYPE2, REQUIRE) \
  PXART_TEMPLATE_BINARY_REQUIRE(CONCEPT1, TYPE1, CONCEPT2, TYPE2, REQUIRE)
#define PXART_TEMPLATE_6(CONCEPT1, TYPE1, CONCEPT2, TYPE2, CONCEPT3, TYPE3) \
  PXART_TEMPLATE_TERNARY(CONCEPT1, TYPE1, CONCEPT2, TYPE2, CONCEPT3, TYPE3)
#define PXART_TEMPLATE_7(CONCEPT1, TYPE1, CONCEPT2, TYPE2, CONCEPT3, TYPE3,  \
                         REQUIRE)                                            \
  PXART_TEMPLATE_TERNARY_REQUIRE(CONCEPT1, TYPE1, CONCEPT2, TYPE2, CONCEPT3, \
                                 TYPE3, REQUIRE)

// Variadic pxart Macro for Supporting Concepts in C++14
#define PXART_TEMPLATE(...) \
  PXART_TEMPLATE_TUPLE((PXART_VAR_NUM_ARGS(__VA_ARGS__), __VA_ARGS__))
#define PXART_TEMPLATE_TUPLE(TUPLE) PXART_TEMPLATE_N_ARGS TUPLE
#define PXART_TEMPLATE_N_ARGS(N, ...) \
  PXART_VAR_GLUE_2(PXART_TEMPLATE_, N)(__VA_ARGS__)

// Simple Concept Support Macro for C++14
#ifdef __cpp_concepts
#define PXART_CONCEPT concept
#else
#define PXART_CONCEPT constexpr bool
#endif

namespace pxart {

namespace meta {

// Utility functions to print the instantiated type when compiling. These
// functions abort the compilation process.
template <typename T>
constexpr auto print_type() = delete;
template <typename T>
constexpr auto print_type(T) = delete;

}  // namespace meta

namespace generic {

// 'typename' Concept to capture every type.
template <typename T>
PXART_CONCEPT type = true;

// Equality Concept
#ifdef __cpp_concepts
template <typename T, typename U>
concept equal = std::same_as<T, U>;
#else   // __cpp_concepts
template <typename T, typename U>
constexpr bool equal = std::is_same_v<T, U>&& std::is_same_v<U, T>;
#endif  // __cpp_concepts

// Integral Concept
#ifdef __cpp_concepts
template <typename T>
concept integral = std::integral<T>;
#else   // __cpp_concepts
template <typename T>
constexpr bool integral = std::is_integral_v<T>;
#endif  // __cpp_concepts

// Unsigned Integral
#ifdef __cpp_concepts
template <typename T>
concept unsigned_integral = std::unsigned_integral<T>;
#else   // __cpp_concepts
template <typename T>
constexpr bool unsigned_integral = std::is_unsigned_v<T>;
#endif  // __cpp_concepts

// Supported Floating-Point Types Concept
template <typename T>
PXART_CONCEPT floating_point = equal<T, float> || equal<T, double>;

// Concept to decide, that a type is not 'void'.
template <typename T>
PXART_CONCEPT not_void = !equal<T, void>;

// Scalar Bit Packet Concept used as return type for PRNGs.
template <typename T>
PXART_CONCEPT scalar_bit_packet = equal<T, uint32_t> || equal<T, uint64_t>;

// Generator Concept
#ifdef __cpp_concepts
template <typename T>
concept generator = requires(T g) {
  { g() }
  ->not_void;
};
#else   // __cpp_concepts
namespace detail {
template <typename T, typename = void>
struct generator : std::false_type {};
template <typename T>
struct generator<T, std::enable_if_t<not_void<decltype(std::declval<T>()())>>>
    : std::true_type {};
}  // namespace detail
template <typename T>
constexpr bool generator = detail::generator<T>::value;
#endif  // __cpp_concepts

}  // namespace generic

namespace meta {

// Returns the return type of a generator with its qualifications, like 'const'.
// template <generic::generator T>
PXART_TEMPLATE(generic::generator, T)
using qualified_result = decltype(std::declval<T>()());

// Returns the return type of a generator without qualification by decaying it.
// template <generic::generator T>
PXART_TEMPLATE(generic::generator, T)
using result = std::decay_t<qualified_result<T>>;

}  // namespace meta

namespace generic {

// Random Bit Generator Concept
template <typename T>
PXART_CONCEPT random_bit_generator =
    generator<T>&& scalar_bit_packet<meta::result<T>>;

// Seeder Concept to decide if type 'T' can be used as a seeder in the seeding
// constructor of type 'U'.
template <typename T, typename U>
PXART_CONCEPT seeder_for =
    random_bit_generator<T> && !equal<std::decay_t<T>, std::decay_t<U>>;

// Concept to decide if a type has a member function, called 'uniform', with
// arguments for bounds and the correct return type.
#ifdef __cpp_concepts
template <typename T, typename R>
concept has_uniform = requires(T t, R a, R b) {
  { t.uniform(a, b) }
  ->equal<R>;
};
#else   // __cpp_concepts
namespace detail {
template <typename T, typename R, typename = void>
struct has_uniform : std::false_type {};
template <typename T, typename R>
struct has_uniform<
    T, R,
    std::enable_if_t<equal<R, decltype(std::declval<T>().uniform(
                                  std::declval<R>(), std::declval<R>()))>>>
    : std::true_type {};
}  // namespace detail
template <typename T, typename R>
constexpr bool has_uniform = detail::has_uniform<T, R>::value;
#endif  // __cpp_concepts

// Concept to decide if a type has a member function template, called 'uniform',
// without arguments for bounds and the correct return type.
#ifdef __cpp_concepts
template <typename T, typename R>
concept has_uniform_01 = requires(T t) {
  { t.template uniform<R>() }
  ->equal<R>;
};
#else   // __cpp_concepts
namespace detail {
template <typename T, typename R, typename = void>
struct has_uniform_01 : std::false_type {};
template <typename T, typename R>
struct has_uniform_01<
    T, R,
    std::enable_if_t<
        equal<R, decltype(std::declval<T>().template uniform<R>())>>>
    : std::true_type {};
}  // namespace detail
template <typename T, typename R>
constexpr bool has_uniform_01 = detail::has_uniform_01<T, R>::value;
#endif  // __cpp_concepts

}  // namespace generic

}  // namespace pxart