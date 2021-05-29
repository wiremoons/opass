# cmake build file for opass
#  created 01 May 2021
#  To use run:  mkdir build && cd build && cmake .. && make
#  On Windows with mingw : mkdir build && cd build && cmake -G "MinGW Makefiles" ..
cmake_minimum_required(VERSION 3.6)
project(opass)
#
# enable verbose output
set(CMAKE_VERBOSE_MAKEFILE on)
#
# add list of C source code files to var ${SOURCES}
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.c")
#
# add location for built binary file
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# set different compiler flags for Windows vs other OS
if(MSVC OR MSYS OR MINGW)
    # for detecting Windows compilers
    # add additional flags to standard ${CMAKE_C_FLAGS} - fix missing srandom/random libs
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -Wall -Dsrandom=srand -Drandom=rand -static")
else()
    # add additional flags to standard ${CMAKE_C_FLAGS}
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -Wall")
endif()

#
# give final executable name and the C source code files required to build it
add_executable(opass ${SOURCES})
#
# list the names of the C libraries to link against: pthreads readline dl
target_link_libraries(opass ${CMAKE_DL_LIBS})
