set(src "${CMAKE_CURRENT_SOURCE_DIR}/src")


add_library(${core_target} SHARED
        #INCLUDE
        ${core_include}/core.h
        ${core_include}/base/types.h
        ${core_include}/tests.h

        #SRC
        ${src}/entrypoint.c
        ${src}/tests.c
)

target_include_directories(${core_target} PUBLIC ${core_include})
