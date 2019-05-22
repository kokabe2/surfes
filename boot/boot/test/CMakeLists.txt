cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_executable(gtests "")
target_sources(gtests
  PRIVATE
    registries/user_system_registry_test.cpp
    registries/user_system_registry_file_test.cpp
    script/system_script_test.cpp
    systems/halt_system_test.cpp
    systems/reboot_system_test.cpp
    systems/system_executor_factory_impl_test.cpp
    ${UTILITY_TEST_DIR}/infrastructures/spy_runtime_error.c
    ${UTILITY_TEST_DIR}/infrastructures/mock_io_data.cpp
)
target_include_directories(gtests
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(gtests
  gtest_main
  surfes_boot_boot
)
add_test(
  NAME
    surfes_boot_boot_test
  COMMAND
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/gtests
)
