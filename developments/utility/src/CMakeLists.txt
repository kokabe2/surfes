cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_library(surfes_utility "")
target_sources(surfes_utility
  PRIVATE
    common/instance_helper.c
    common/list_node.c
    common/list.c
    domains/sif_file.c
    domains/sif_file_validator.c
    infrastructures/led_driver.c
    infrastructures/micro_time.c
    infrastructures/ssd_driver.c
    services/modular_sum.c
    services/sjis_code_converter.c
)
target_include_directories(surfes_utility
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${ITS_INCLUDE_DIR}
)
