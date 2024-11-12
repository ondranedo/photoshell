# Core cmake files

cmake_minimum_required(VERSION 3.5)

project(Photoshell)

set(build ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${build})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${build})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${build}/lib)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED true)

set(core_target core)
set(core_include "${CMAKE_SOURCE_DIR}/core/include")
add_subdirectory(core)

set(photoshell_target photoshell)
add_subdirectory(photoshell)

