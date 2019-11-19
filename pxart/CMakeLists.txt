get_filename_component(library_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)

find_package(fmt REQUIRED)
find_package(doctest REQUIRED)

add_library(${library_name} INTERFACE)
add_executable(internal-tests)

add_subdirectory(test)
add_subdirectory(utilities)
add_subdirectory(simd256)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  target_compile_options(${library_name} INTERFACE -mrdrnd)
endif()

target_include_directories(${library_name}
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/..>
)
target_compile_features(${library_name} INTERFACE cxx_std_17)
target_link_libraries(${library_name} INTERFACE fmt::fmt)
target_compile_options(${library_name} INTERFACE -march=native)
add_library(${library_name}::${library_name} ALIAS ${library_name})

# internal tests
target_sources(internal-tests
  PRIVATE
    main.test.cpp
    bit_reversal.test.cpp
    mt19937.test.cpp
)

target_link_libraries(internal-tests
  PRIVATE
    doctest::doctest
    ${library_name}::${library_name}
)

add_test(internal-tests internal-tests)

# install
include(GNUInstallDirs)

install(
  TARGETS ${library_name}
  EXPORT ${library_name}Targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
  DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING PATTERN "*.hpp"
)

install(
  EXPORT ${library_name}Targets
  NAMESPACE ${library_name}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${library_name}
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${library_name}Config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${library_name}
)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${library_name}Version.cmake
  COMPATIBILITY SameMajorVersion
)

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${library_name}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${library_name}Version.cmake
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/${library_name}
)

export(
  TARGETS ${library_name}
  NAMESPACE ${library_name}::
  FILE ${library_name}Targets.cmake
)

export(PACKAGE ${library_name})