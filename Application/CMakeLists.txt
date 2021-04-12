project(Application)

aux_source_directory(. DIR_SRCS)
file (GLOB_RECURSE  DIR_SRCS *.cpp *.cc *.c *.h *.hpp)
add_executable(Application ${DIR_SRCS})

target_link_libraries(Application PRIVATE PhantomCore 
                                          ${Vulkan_LIBRARIES} 
                                          glfw)

add_custom_command(TARGET Application POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/../Resource
    ${CMAKE_CURRENT_BINARY_DIR}/Resource
)
