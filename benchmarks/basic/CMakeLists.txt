add_executable(monte_carlo_pi monte_carlo_pi.cpp)
target_link_libraries(monte_carlo_pi
  PRIVATE
    pxart::pxart
    perfevent::perfevent
)
add_test(monte_carlo_pi monte_carlo_pi)

add_executable(generate_cache generate_cache.cpp)
target_link_libraries(generate_cache
  PRIVATE
    pxart::pxart
    perfevent::perfevent
)
add_test(generate_cache generate_cache)