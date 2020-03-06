if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
  cmake_minimum_required(VERSION 3.13)
  project(pxart-tests
    VERSION 0.1.0
    DESCRIPTION
      "Application for Direct Usage of pXart RNGs"
    HOMEPAGE_URL
      "https://github.com/lyrahgames/pxart"
    LANGUAGES CXX C
  )
  message(STATUS "Package 'pxrun' will be built separately.")
  enable_testing()
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../cmake")
  include(DefaultBuildType)
  find_package(pxart REQUIRED)
  find_package(TestU01 REQUIRED)
  find_package(Boost REQUIRED COMPONENTS program_options)
  add_subdirectory(pxrun)
else()
  find_package(pxart)
  find_package(TestU01)
  find_package(Boost COMPONENTS program_options)
  if (pxart_FOUND AND Boost_FOUND AND TestU01_FOUND)
    add_subdirectory(pxrun)
  endif()
endif()
