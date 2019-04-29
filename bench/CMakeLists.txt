find_package(Celero REQUIRED)

add_executable(main-benchmarks)

target_sources(main-benchmarks
  PRIVATE
    main.bench.cpp
    test.bench.cpp
)

target_link_libraries(main-benchmarks
  PRIVATE
    ${CELERO_LIBRARIES}
)

target_include_directories(main-benchmarks
  PRIVATE
    ${CELERO_INCLUDE_DIRS}
)