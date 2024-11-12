set(src ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(${photoshell_target} 
    ${src}/entrypoint.c
)

target_link_libraries(${photoshell_target} PUBLIC ${core_target})
target_include_directories(${photoshell_target} PRIVATE ${core_include})
