cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_library(surfes_utility "")
target_sources(surfes_utility
  PRIVATE
    common/instance_helper.c
    common/list_node.c
    common/list.c
    domains/sjis_code_converter.c
    infrastructures/drivers/led_driver.c
    infrastructures/drivers/micro_time.c
    infrastructures/drivers/ssd_driver.c
    infrastructures/frameworks/sif_file.c
    infrastructures/frameworks/sif_file_validator.c
    services/modular_sum.c
)
target_include_directories(surfes_utility
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${ITS_INCLUDE_DIR}
)
