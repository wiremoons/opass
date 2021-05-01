# cmake build file for opass
#  created 01 May 2021
#  To use run:  mkdir build && cd build && cmake .. && make  
cmake_minimum_required(VERSION 3.6)
project(opass)
#
# enable verbose output
#set(CMAKE_VERBOSE_MAKEFILE on)
#
# add list of c source code files to var ${SOURCES}
file(GLOB SOURCES "./src/*.c")
#
# add location for built binary file
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
#
# add additional flags to standard ${CMAKE_C_FLAGS}
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -Wall")
#
# give final executable name and the C source code files required to build it
add_executable(opass ${SOURCES})
#
# list the names of the C libraries to link against: pthreads readline dl
target_link_libraries(opass ${CMAKE_DL_LIBS})
