cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_executable(gtests "")
target_sources(gtests
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/common/spy_runtime_error.c
    ${CMAKE_CURRENT_LIST_DIR}/infrastructures/drivers/led_driver_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/infrastructures/drivers/ssd_driver_test.cpp
)
target_include_directories(gtests
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(gtests
  gtest_main
  surfes
)
add_test(
  NAME
    surfes_test
  COMMAND
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/gtests
)
