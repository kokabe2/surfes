cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_executable(gtests "")
target_sources(gtests
  PRIVATE
    diagnosis/memory_diagnostic_transaction_test.cpp
    diagnosis/memory_diagnostician_factory_test.cpp
    ${UTILITY_TEST_DIR}/infrastructures/mock_io_data.cpp
)
target_include_directories(gtests
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(gtests
  gtest_main
  surfes_core_boot
)
add_test(
  NAME
    surfes_core_boot_test
  COMMAND
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/gtests
)
