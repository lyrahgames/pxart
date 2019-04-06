find_package(doctest REQUIRED)

add_executable(internal-tests)
target_sources(internal-tests
  PRIVATE
    main.test.cpp
)
target_link_libraries(internal-tests
  PRIVATE
    doctest::doctest
)