cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_library(surfes_utility "")
target_sources(surfes_utility
  PRIVATE
    domains/sif_file_base.c
    domains/sif_file_validator.c
    infrastructures/instance_helper.c
    services/modular_sum.c
)
target_include_directories(surfes_utility
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../include
)
