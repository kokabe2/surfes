cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(surfes_core_kernel)

# place binaries and libraries according to GNU standards
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# we use this to get code coverage
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fprofile-arcs -ftest-coverage")
  set(CMAKE_C_FLAGS "-std=c99")
elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set(CORE_KERNEL_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(CORE_KERNEL_UTKERNEL_DIR ${CMAKE_CURRENT_LIST_DIR}/test/utkernel)
set(UTILITY_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../utility/src)
set(UTILITY_TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/../utility/test)
set(UTILITY_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/../utility/include)

add_subdirectory(src)

include(cmake/googletest.cmake)
fetch_googletest(
  ${PROJECT_SOURCE_DIR}/cmake
  ${PROJECT_SOURCE_DIR}/../thirdparty/google/googletest
)

enable_testing()
add_subdirectory(test)
