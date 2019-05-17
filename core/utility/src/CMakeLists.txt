cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_library(surfes_core_utility "")
target_sources(surfes_core_utility
  PRIVATE
    utilities/instance_helper.c
)
target_include_directories(surfes_core_utility
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../include
)
