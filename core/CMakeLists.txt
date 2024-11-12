set(src "${CMAKE_CURRENT_SOURCE_DIR}/src")


add_library(${core_target} SHARED 
    ${src}/foo.c
    ${core_include}/foo.h
)

target_include_directories(${core_target} PUBLIC ${core_include})
