cmake_minimum_required(VERSION 3.13)
project(random-number-generators VERSION 0.1.0 LANGUAGES CXX C)
enable_testing()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Test endianness of system.
include(TestBigEndian)
test_big_endian(is_big_endian)
if (${is_big_endian})
  message(STATUS "The target system is big endian.")
else()
  message(STATUS "The target system is little endian.")
endif()

# Set the default build type to 'Release' and check for a valid build type.
if (NOT GENERATOR_IS_MULTI_CONFIG)
  set(allowable_build_types Debug Release RelWithDebInfo MinSizeRel)
  set(default_build_type Release)
  set_property(
    CACHE
      CMAKE_BUILD_TYPE
    PROPERTY
      STRINGS "${allowable_build_types}"
  )
  if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE ${default_build_type} CACHE STRING "" FORCE)
    message(
      STATUS "Setting build type to '${default_build_type}' as none was specified.")
  elseif(NOT CMAKE_BUILD_TYPE IN_LIST allowable_build_types)
    message(FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}")
  endif()
endif()

add_subdirectory(extern)
add_subdirectory(pxart)
add_subdirectory(rng)
add_subdirectory(bench)