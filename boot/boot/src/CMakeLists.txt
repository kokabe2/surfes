cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_library(surfes_boot_boot "")
target_sources(surfes_boot_boot
  PRIVATE
    bootstrapper.c
    application/boot_application.c
    config/file_header.c
    platforms/exception_event.c
    platforms/microcontroller.c
    platforms/platform.c
    platforms/section.c
    registries/user_system_registry.c
    registries/user_system_registry_file.c
    script/system_script.c
    systems/halt_system.c
    systems/reboot_system.c
    systems/system_executor_factory_impl.c
    ${UTILITY_SOURCE_DIR}/domains/sif_file_base.c
    ${UTILITY_SOURCE_DIR}/domains/sif_file_validator.c
    ${UTILITY_SOURCE_DIR}/infrastructures/instance_helper.c
    ${UTILITY_SOURCE_DIR}/services/modular_sum.c
)
target_include_directories(surfes_boot_boot
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${BOOT_BOOT_INCLUDE_DIR}
    ${UTILITY_INCLUDE_DIR}
)
