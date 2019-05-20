cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_executable(gtests "")
target_sources(gtests
  PRIVATE
    domains/sif_file_base_test.cpp
    domains/sif_file_validator_test.cpp
    infrastructures/instance_helper_test.cpp
    infrastructures/spy_runtime_error.c
    services/modular_sum_test.cpp
)
target_include_directories(gtests
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(gtests
  gtest_main
  surfes_utility
)
add_test(
  NAME
    surfes_utility_test
  COMMAND
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/gtests
)
