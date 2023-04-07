cmake_minimum_required(VERSION 3.0.0)

project(cmd)

set(CMAKE_VERBOSE_MAKEFILE on)

if (WIN32)
    add_definitions(-DWIN32)
    set(OUT_DIR ${CMAKE_SOURCE_DIR}/bin/win/64)
elseif (CMAKE_COMPILER_IS_GNUCC)
    set(OUT_DIR ${CMAKE_SOURCE_DIR}/bin/linux/64)
elseif(APPLE)
    set(OUT_DIR ${CMAKE_SOURCE_DIR}/bin/mac/64)
endif (WIN32)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR}/${CMAKE_BUILD_TYPE} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUT_DIR}/${CMAKE_BUILD_TYPE} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUT_DIR}/${CMAKE_BUILD_TYPE} )


add_executable(test src/main.cpp)