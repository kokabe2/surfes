cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_library(surfes_boot "")
target_sources(surfes_boot
  PRIVATE
    common/instance_helper.c
    common/modular_sum.c
    domains/sif_file.c
    domains/sif_file_validator.c
    infrastructures/drivers/led_driver.c
    infrastructures/drivers/ssd_driver.c
)
target_include_directories(surfes_boot
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
