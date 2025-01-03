# Core cmake files
cmake_minimum_required(VERSION 3.29)
project(Photoshell)

# Setting VARIABLES

## Build directory
set(build ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${build})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${build})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${build}/lib)

add_compile_options(-Wall -Wextra -Wno-unused-variable -Wuninitialized -Werror=vla)

## Language standard
set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED true)

# Setting DEFINES
if(UNIX AND NOT APPLE)
    add_compile_definitions(PLATFORM_LINUX)
endif()

# Subdirectories
## BINARY FILE names
set(core_target core)
set(photoshell_target photoshell)

## CORE library
set(core_include "${CMAKE_SOURCE_DIR}/core/include")
add_subdirectory(core)

## PHOTOSHELL editor
add_subdirectory(photoshell)

