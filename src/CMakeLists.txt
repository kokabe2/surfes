cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
add_library(surfes "")
target_sources(surfes
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/infrastructures/drivers/led_driver.c
    ${CMAKE_CURRENT_LIST_DIR}/infrastructures/drivers/ssd_driver.c
)
target_include_directories(surfes
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
