if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
  cmake_minimum_required(VERSION 3.13)
  project(pxart-tests
    VERSION 0.1.0
    DESCRIPTION
      "Benchmarks for pXart"
    HOMEPAGE_URL
      "https://github.com/lyrahgames/pxart"
    LANGUAGES CXX C
  )
  message(STATUS "Package 'pxart-benchmarks' will be built separately.")
  enable_testing()
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../cmake")
  include(DefaultBuildType)
  find_package(pxart REQUIRED)
  find_package(perfevent REQUIRED)
  find_package(TestU01 REQUIRED)
  add_subdirectory(basic)
  add_subdirectory(statistical_performance)
else()
  find_package(perfevent)
  find_package(TestU01)
  if (perfevent_FOUND AND TestU01_FOUND)
    add_subdirectory(basic)
    add_subdirectory(statistical_performance)
  endif()
endif()

