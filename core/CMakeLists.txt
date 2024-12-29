set(src "${CMAKE_CURRENT_SOURCE_DIR}/src")


add_library(${core_target} SHARED
        #INCLUDE
        ${core_include}/core.h
        ${core_include}/base/types.h
        ${core_include}/base/types.h
        ${core_include}/error.h
        ${core_include}/application.h
        ${core_include}/argument.h
        ${core_include}/image.h

        #SRC
        ${src}/entrypoint.c
        ${src}/application.c
        ${src}/string.c
        ${src}/argument.c
        ${src}/error.c
        src/image.c
)

target_include_directories(${core_target} PUBLIC ${core_include})
target_link_libraries(${core_target} PRIVATE m)