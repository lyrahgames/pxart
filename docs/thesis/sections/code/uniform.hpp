constexpr auto has_uniform_01 =
    is_valid([](auto&& x) -> decltype(x.uniform()) {});
constexpr auto has_uniform =
    is_valid([](auto&& x, auto&& y, auto&& z) -> decltype(x.uniform(y, z)) {});

template <typename Real, typename RNG>
constexpr inline auto uniform(RNG&& rng) noexcept
    -> std::enable_if_t<!decltype(has_uniform_01(rng))::value, Real> {
  return detail::uniform<Real>(std::forward<RNG>(rng)());
}

template <typename Real, typename RNG>
constexpr inline auto uniform(RNG&& rng) noexcept
    -> std::enable_if_t<decltype(has_uniform_01(rng))::value, Real> {
  return std::forward<RNG>(rng).uniform();
}

template <typename Real, typename RNG>
constexpr inline auto uniform(RNG&& rng, Real a, Real b) noexcept
    -> std::enable_if_t<!decltype(has_uniform(rng, a, b))::value, Real> {
  return detail::uniform(std::forward<RNG>(rng)(), a, b);
}

template <typename Real, typename RNG>
constexpr inline auto uniform(RNG&& rng, Real a, Real b) noexcept
    -> std::enable_if_t<decltype(has_uniform(rng, a, b))::value, Real> {
  return std::forward<RNG>(rng).uniform(a, b);
}

template <typename Real>
constexpr inline Real uniform(std::mt19937& rng, Real a = 0,
                              Real b = 1) noexcept {
  return detail::uniform(static_cast<uint32_t>(rng()), a, b);
}

template <typename Real>
constexpr inline Real uniform(std::mt19937&& rng, Real a = 0,
                              Real b = 1) noexcept {
  return detail::uniform(static_cast<uint32_t>(std::move(rng)()), a, b);
}