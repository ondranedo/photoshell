set(src "${CMAKE_CURRENT_SOURCE_DIR}/src")


add_library(${core_target} SHARED
        #INCLUDE
        ${core_include}/base/string.h
        ${core_include}/base/types.h
        ${core_include}/core.h
        ${core_include}/entryparam.h
        ${core_include}/tests.h
        ${core_include}/errors.h
        ${core_include}/application.h

        #SRC
        ${src}/entrypoint.c
        ${src}/tests.c
        ${src}/application.c
        ${src}/string.c
        ${src}/entryparam/entryparam.h
        ${src}/entryparam/entryparam.c
)

target_include_directories(${core_target} PUBLIC ${core_include})
