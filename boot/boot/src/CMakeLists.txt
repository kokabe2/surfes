cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_library(surfes_boot "")
target_sources(surfes_boot
  PRIVATE
    bootstrapper.c
    application/boot_application.c
    drivers/led_driver.c
    drivers/ssd_driver.c
    filer/sif_file_base.c
    filer/sif_file_validator.c
    platforms/exception_event.c
    platforms/microcontroller.c
    platforms/platform.c
    platforms/section.c
    script/system_script.c
    systems/halt_system.c
    systems/reboot_system.c
    systems/sys_exe_file.c
    systems/system_executor_factory_impl.c
    systems/update_system.c
    systems/user_system.c
    utilities/instance_helper.c
    utilities/modular_sum.c
)
target_include_directories(surfes_boot
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
