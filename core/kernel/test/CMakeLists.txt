cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_executable(gtests "")
target_sources(gtests
  PRIVATE
    utkernel/inbox_test.cpp
    utkernel/isr_test.cpp
    utkernel/task_test.cpp
    utkernel/impl/fake_int.c
    utkernel/impl/fake_mailbox.c
    utkernel/impl/fake_memorypool.c
    utkernel/impl/fake_task.c
)
target_include_directories(gtests
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(gtests
  gtest_main
  surfes_core_kernel
)
add_test(
  NAME
    surfes_core_kernel_test
  COMMAND
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/gtests
)
