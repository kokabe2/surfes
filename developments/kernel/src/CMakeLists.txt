cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_library(surfes_core_kernel "")
target_sources(surfes_core_kernel
  PRIVATE
    utkernel/heap.c
    utkernel/inbox.c
    utkernel/interval_timer.c
    utkernel/isr.c
    utkernel/one_shot_timer.c
    utkernel/system_time.c
    utkernel/task.c
    utkernel/timer.c
    ${UTILITY_SOURCE_DIR}/common/instance_helper.c
    ${UTILITY_TEST_DIR}/common/spy_runtime_error.c
)
target_include_directories(surfes_core_kernel
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CORE_KERNEL_INCLUDE_DIR}
    ${CORE_KERNEL_UTKERNEL_DIR}
    ${UTILITY_INCLUDE_DIR}
)
