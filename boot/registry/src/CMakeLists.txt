cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_library(surfes_boot_registry "")
target_sources(surfes_boot_registry
  PRIVATE
    registry_api.c
    config/file_header.c
    database/int_registry_getter.c
    database/uintptr_registry_getter.c
    ${UTILITY_SOURCE_DIR}/domains/sif_file.c
    ${UTILITY_SOURCE_DIR}/domains/sif_file_validator.c
    ${UTILITY_SOURCE_DIR}/infrastructures/instance_helper.c
    ${UTILITY_SOURCE_DIR}/services/modular_sum.c
)
target_include_directories(surfes_boot_registry
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${BOOT_REGISTRY_INCLUDE_DIR}
    ${UTILITY_INCLUDE_DIR}
)
