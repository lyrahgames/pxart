find_package(fmt REQUIRED)

add_executable(random_device_info)
target_sources(random_device_info
  PRIVATE
    random_device_info.cpp
)
target_link_libraries(random_device_info
  PRIVATE
    fmt::fmt
    librng::librng
)

add_executable(middle_square)
target_sources(middle_square
  PRIVATE
    middle_square.cpp
)
target_link_libraries(middle_square
  PRIVATE
    fmt::fmt
    librng::librng
)