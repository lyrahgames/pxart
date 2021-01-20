#pragma once
#include <concepts>
#include <functional>

namespace pxart {

namespace meta {

template <typename T>
constexpr auto print_type() = delete;

template <typename T>
constexpr auto print_type(T) = delete;

}  // namespace meta

namespace generic {

template <typename T>
concept integral = std::integral<T>;

template <typename T>
concept unsigned_integral = std::unsigned_integral<T>;

template <typename T>
concept floating_point = std::same_as<T, float> || std::same_as<T, double>;

template <typename T>
concept scalar_bit_packet =
    std::same_as<T, uint32_t> || std::same_as<T, uint64_t>;

template <typename T>
concept not_void = !std::same_as<T, void>;

template <typename T>
concept generator = requires(T g) {
  { g() }
  ->not_void;
};

}  // namespace generic

namespace meta {

template <generic::generator T>
using qualified_result = decltype(std::declval<T>()());

template <generic::generator T>
using result = std::decay_t<qualified_result<T>>;

}  // namespace meta

namespace generic {

template <typename T>
concept random_bit_generator =
    generator<T>&& scalar_bit_packet<meta::result<T>>;

template <typename T, typename U>
concept seeder_for =
    random_bit_generator<T> && !std::same_as<std::decay_t<T>, std::decay_t<U>>;

template <typename T, typename R>
concept has_uniform = requires(T t, R a, R b) {
  { t.uniform(a, b) }
  ->std::same_as<R>;
};

template <typename T, typename R>
concept has_uniform_01 = requires(T t) {
  { t.template uniform<R>() }
  ->std::same_as<R>;
};

}  // namespace generic

}  // namespace pxart