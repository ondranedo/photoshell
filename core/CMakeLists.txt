set(src "${CMAKE_CURRENT_SOURCE_DIR}/src")


add_library(${core_target} SHARED
        #INCLUDE
        ${core_include}/core.h
        ${core_include}/base/types.h
        ${core_include}/base/types.h
        ${core_include}/error.h
        ${core_include}/application.h
        ${core_include}/arguments.h

        #SRC
        ${src}/entrypoint.c
        ${src}/application.c
        ${src}/string.c
        ${src}/arguments.c
        ${src}/error.c
)

target_include_directories(${core_target} PUBLIC ${core_include})
