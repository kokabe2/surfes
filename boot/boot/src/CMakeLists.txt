cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_library(surfes_boot "")
target_sources(surfes_boot
  PRIVATE
    common/instance_helper.c
    common/modular_sum.c
    infrastructures/frameworks/core_file.c
    infrastructures/frameworks/sif_file.c
    infrastructures/frameworks/sif_file_validator.c
    infrastructures/frameworks/system_runner.c
    infrastructures/frameworks/updater_file.c
)
target_include_directories(surfes_boot
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
