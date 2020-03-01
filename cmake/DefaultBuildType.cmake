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