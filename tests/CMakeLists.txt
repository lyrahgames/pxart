if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
  cmake_minimum_required(VERSION 3.13)
  project(pxart-tests
    VERSION 0.1.0
    DESCRIPTION
      "Tests for pXart"
    HOMEPAGE_URL
      "https://github.com/lyrahgames/pxart"
    LANGUAGES CXX C
  )
  message(STATUS "Package 'pxart-tests' will be built separately.")
  enable_testing()
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../cmake")
  include(DefaultBuildType)
  find_package(pxart REQUIRED)
  find_package(doctest REQUIRED)
  add_subdirectory(basics)
else()
  find_package(doctest)
  if (doctest_FOUND)
    add_subdirectory(basics)
  endif()
endif()
