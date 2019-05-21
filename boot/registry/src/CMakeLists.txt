cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_library(surfes_boot_registry "")
target_sources(surfes_boot_registry
  PRIVATE
    registries/int_registry_getter.c
    registries/uintptr_registry_getter.c
    config/file_header.c
)
target_include_directories(surfes_boot_registry
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${BOOT_REGISTRY_INCLUDE_DIR}
    ${UTILITY_INCLUDE_DIR}
)
