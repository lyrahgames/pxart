add_executable(monte_carlo_pi monte_carlo_pi.cpp)
target_link_libraries(monte_carlo_pi
  PRIVATE
    pxart::pxart
    perfevent::perfevent
)

add_test(monte_carlo_pi monte_carlo_pi)