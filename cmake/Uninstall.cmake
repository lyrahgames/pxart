configure_file(
  ${CMAKE_CURRENT_LIST_DIR}/Uninstall.cmake.in
  Uninstall.cmake
  @ONLY
)

add_custom_target(
  uninstall
  COMMAND
    ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/Uninstall.cmake
)