cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_executable(gtests "")
target_sources(gtests
  PRIVATE
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
  surfes_core_utility
)
add_test(
  NAME
    surfes_core_utility_test
  COMMAND
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/gtests
)
