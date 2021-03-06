cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_executable(gtests "")
target_sources(gtests
  PRIVATE
    registries/runlevel_provider_test.cpp
    registries/user_system_file_provider_impl_test.cpp
    script/system_script_test.cpp
    systems/halt_system_test.cpp
    systems/reboot_system_test.cpp
    systems/system_executor_factory_impl_test.cpp
    systems/user_system_test.cpp
    ${UTILITY_TEST_DIR}/common/spy_runtime_error.c
    ${UTILITY_TEST_DIR}/infrastructures/drivers/mock_io_data.cpp
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
