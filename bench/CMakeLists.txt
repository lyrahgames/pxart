find_package(Boost REQUIRED)
find_package(Celero REQUIRED)

add_executable(main-benchmarks)

target_sources(main-benchmarks
  PRIVATE
    main.bench.cpp
    test.bench.cpp
)

target_link_libraries(main-benchmarks
  PRIVATE
    Celero::Celero
)

add_executable(perfevent_std_mt19937 std_mt19937.cpp)
target_link_libraries(perfevent_std_mt19937 PRIVATE perfevent::perfevent)

add_executable(gen gen.cpp)
target_link_libraries(gen PRIVATE Celero::Celero perfevent::perfevent Boost::boost pxart::pxart)

add_executable(monte_carlo_pi monte_carlo_pi.cpp)
target_link_libraries(monte_carlo_pi PRIVATE Celero::Celero perfevent::perfevent Boost::boost pxart::pxart)
target_compile_options(monte_carlo_pi PRIVATE -O3)

add_executable(generate generate.cpp)
target_link_libraries(generate
  PRIVATE
    Celero::Celero
    perfevent::perfevent
    Boost::boost
    pxart::pxart
)
target_compile_options(generate PRIVATE -O3)