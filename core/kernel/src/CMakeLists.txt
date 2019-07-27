cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_library(surfes_core_kernel "")
target_sources(surfes_core_kernel
  PRIVATE
    utkernel/inbox.c
    utkernel/isr.c
    utkernel/task.c
    ${UTILITY_SOURCE_DIR}/infrastructures/instance_helper.c
    ${UTILITY_SOURCE_DIR}/infrastructures/runtime_error.c
)
target_include_directories(surfes_core_kernel
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CORE_KERNEL_INCLUDE_DIR}
    ${CORE_KERNEL_UTKERNEL_DIR}
    ${UTILITY_INCLUDE_DIR}
)
