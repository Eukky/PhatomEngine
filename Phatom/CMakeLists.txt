project(Phatom)
set(COMMON_FILES 
    Common/File.h
    Common/TextFileReader.h
    Common/UnicodeUtil.h

    Common/File.cpp
    Common/TextFileReader.cpp
    Common/UnicodeUtil.cpp)

set(CORE_FILES 
    Core/Window.h

    Core/PhatomMain.cpp
    Core/Window.cpp)

set(MMD_FILES 
    Model/MMD/PMXFile.h
    Model/MMD/PMXFileReader.h
    
    Model/MMD/PMXFileReader.cpp
    Model/MMD/PMXFile.cpp)

set(GRAPHICS_FILES 
    Graphics/VulkanDevice.h
    Graphics/VulkanInstance.h
    Graphics/VulkanDevice.cpp
    Graphics/VulkanInstance.cpp)

source_group(Common FILES ${COMMON_FILES})

source_group(Core FILES ${CORE_FILES})

source_group(Model/MMD FILES ${MMD_FILES})

source_group(Graphics FILES ${GRAPHICS_FILES})

set(PROJECT_FILES 
    ${COMMON_FILES}
    ${CORE_FILES}
    ${MMD_FILES}
    ${GRAPHICS_FILES})

add_executable(${PROJECT_NAME} ${PROJECT_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES}
                                      glfw)

add_custom_command(TARGET Phatom POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/../Resource
    ${CMAKE_CURRENT_BINARY_DIR}/Resource)