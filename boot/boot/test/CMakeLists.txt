cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_executable(gtests "")
target_sources(gtests
  PRIVATE
    common/instance_helper_test.cpp
    common/modular_sum_test.cpp
    common/spy_runtime_error.c
    domains/sif_file_test.cpp
    domains/sif_file_validator_test.cpp
    infrastructures/drivers/led_driver_test.cpp
    infrastructures/drivers/ssd_driver_test.cpp
)
target_include_directories(gtests
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(gtests
  gtest_main
  surfes_boot
)
add_test(
  NAME
    surfes_boot_test
  COMMAND
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/gtests
)
