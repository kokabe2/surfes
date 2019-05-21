cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_executable(gtests "")
target_sources(gtests
  PRIVATE
    registries/int_registry_getter_test.cpp
    registries/uintptr_registry_getter_test.cpp
    ${UTILITY_TEST_DIR}/infrastructures/spy_runtime_error.c
)
target_include_directories(gtests
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(gtests
  gtest_main
  surfes_boot_registry
)
add_test(
  NAME
    surfes_boot_registry_test
  COMMAND
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/gtests
)
