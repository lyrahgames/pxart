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
target_link_libraries(monte_carlo_pi
  PRIVATE
    Celero::Celero
    perfevent::perfevent
    Boost::boost
    pxart::pxart
)
target_compile_options(monte_carlo_pi PRIVATE -O3)

add_executable(mcp mcp.cpp)
target_link_libraries(mcp
  PRIVATE
    perfevent::perfevent
    Boost::boost
    pxart::pxart
)
target_compile_options(mcp PRIVATE -O3)

add_executable(generate generate.cpp)
target_link_libraries(generate
  PRIVATE
    Celero::Celero
    perfevent::perfevent
    Boost::boost
    pxart::pxart
)
target_compile_options(generate PRIVATE -O3)

add_executable(generate_cache generate_cache.cpp)
target_link_libraries(generate_cache
  PRIVATE
    perfevent::perfevent
    Boost::boost
    pxart::pxart
)
target_compile_options(generate_cache PRIVATE -O3)

add_executable(mt19937_bitstream mt19937_bitstream.cpp)
target_link_libraries(mt19937_bitstream PRIVATE pxart::pxart)
target_compile_options(mt19937_bitstream PRIVATE -O3)

add_executable(mt19937_simd256_bitstream mt19937_simd256_bitstream.cpp)
target_link_libraries(mt19937_simd256_bitstream PRIVATE pxart::pxart)
target_compile_options(mt19937_simd256_bitstream PRIVATE -O3)

add_executable(xrsr128p_bitstream xrsr128p_bitstream.cpp)
target_link_libraries(xrsr128p_bitstream PRIVATE pxart::pxart)
target_compile_options(xrsr128p_bitstream PRIVATE -O3)

add_executable(xrsr128p_simd256_bitstream xrsr128p_simd256_bitstream.cpp)
target_link_libraries(xrsr128p_simd256_bitstream PRIVATE pxart::pxart)
target_compile_options(xrsr128p_simd256_bitstream PRIVATE -O3)

add_executable(msws_bitstream msws_bitstream.cpp)
target_link_libraries(msws_bitstream PRIVATE pxart::pxart)
target_compile_options(msws_bitstream PRIVATE -O3)

add_executable(msws_simd256_bitstream msws_simd256_bitstream.cpp)
target_link_libraries(msws_simd256_bitstream PRIVATE pxart::pxart)
target_compile_options(msws_simd256_bitstream PRIVATE -O3)


find_package(TestU01 REQUIRED)

add_executable(xrsr128p_testu01 xrsr128p_testu01.cpp)
target_link_libraries(xrsr128p_testu01
  PRIVATE
    pxart::pxart
    TestU01::TestU01
)
target_compile_options(xrsr128p_testu01 PRIVATE -O3)

add_executable(xrsr128p_simd256_testu01 xrsr128p_simd256_testu01.cpp)
target_link_libraries(xrsr128p_simd256_testu01
  PRIVATE
    pxart::pxart
    TestU01::TestU01
)
target_compile_options(xrsr128p_simd256_testu01 PRIVATE -O3)

add_executable(msws_testu01 msws_testu01.cpp)
target_link_libraries(msws_testu01
  PRIVATE
    pxart::pxart
    TestU01::TestU01
)
target_compile_options(msws_testu01 PRIVATE -O3)

add_executable(msws_simd256_testu01 msws_simd256_testu01.cpp)
target_link_libraries(msws_simd256_testu01
  PRIVATE
    pxart::pxart
    TestU01::TestU01
)
target_compile_options(msws_simd256_testu01 PRIVATE -O3)