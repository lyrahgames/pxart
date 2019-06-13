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