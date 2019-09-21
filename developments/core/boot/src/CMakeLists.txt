cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_library(surfes_core_boot "")
target_sources(surfes_core_boot
  PRIVATE
    bootstrapper.c
    config/file_header.c
    diagnosis/memory_diagnostic_transaction.c
    diagnosis/memory_diagnostician_factory.c
    ${BOOT_REGISTRY_SOURCE_DIR}/registry_api.c
    ${BOOT_REGISTRY_SOURCE_DIR}/database/int_registry_getter.c
    ${BOOT_REGISTRY_SOURCE_DIR}/database/uintptr_registry_getter.c
    ${UTILITY_SOURCE_DIR}/common/instance_helper.c
    ${UTILITY_SOURCE_DIR}/common/list.c
    ${UTILITY_SOURCE_DIR}/infrastructures/frameworks/sif_file.c
    ${UTILITY_SOURCE_DIR}/infrastructures/frameworks/sif_file_validator.c
    ${UTILITY_SOURCE_DIR}/services/modular_sum.c
)
target_include_directories(surfes_core_boot
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${ITS_INCLUDE_DIR}
    ${BOOT_REGISTRY_INCLUDE_DIR}
    ${UTILITY_INCLUDE_DIR}
)
