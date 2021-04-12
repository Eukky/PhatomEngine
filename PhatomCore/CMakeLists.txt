project(PhantomCore)

set(COMMON_MODULE_NAME "Common")
set(CORE_MODULE_NAME "Core")
set(MODEL_MODULE_NAME "Model")
set(GRAPHICS_MODULE_NAME "Graphics")

set(INCLUDE_DIR ${CORE_MODULE_NAME}/include
                ${COMMON_MODULE_NAME}/include
                ${MODEL_MODULE_NAME}/include
                ${GRAPHICS_MODULE_NAME}/include)

include_directories(${INCLUDE_DIR})

set(COMMON_INCLUDE_FILES ${COMMON_MODULE_NAME}/include/PFile.h)
set(COMMON_SRC_FILES ${COMMON_MODULE_NAME}/src/PFile.cpp)

set(CORE_INCLUDE_FILES ${CORE_MODULE_NAME}/include/PWindow.h)
set(CORE_SRC_FILES ${CORE_MODULE_NAME}/src/PWindow.cpp)

set(MODEL_INCLUDE_FILES ${MODEL_MODULE_NAME}/include/*.h)
set(MODEL_SRC_FILES ${MODEL_MODULE_NAME}/src/*.cpp)

set(GRAPHICS_INCLUDE_FILES ${GRAPHICS_MODULE_NAME}/include/*.h)
set(GRAPHICS_SRC_FILES ${GRAPHICS_MODULE_NAME}/src/*.cpp)

source_group(${COMMON_MODULE_NAME}/include FILES ${COMMON_INCLUDE_FILES})
source_group(${COMMON_MODULE_NAME}/src FILES ${COMMON_SRC_FILES})

source_group(${CORE_MODULE_NAME}/include FILES ${CORE_INCLUDE_FILES})
source_group(${CORE_MODULE_NAME}/src FILES ${CORE_SRC_FILES})

source_group(${MODEL_MODULE_NAME}/include FILES ${MODEL_INCLUDE_FILES})
source_group(${MODEL_MODULE_NAME}/src FILES ${MODEL_SRC_FILES})

source_group(${GRAPHICS_MODULE_NAME}/include FILES ${GRAPHICS_INCLUDE_FILES})
source_group(${GRAPHICS_MODULE_NAME}/src FILES ${GRAPHICS_SRC_FILES})

set(HEADER_FILES_DIR ${CORE_INCLUDE_FILES}
                     ${COMMON_INCLUDE_FILES}
                     ${MODEL_INCLUDE_FILES}
                     ${GRAPHICS_INCLUDE_FILES})

set(SOURCE_FILES_DIR ${CORE_SRC_FILES}
                     ${COMMON_SRC_FILES}
                     ${MODEL_INCLUDE_FILES}
                     ${GRAPHICS_INCLUDE_FILES})

file(GLOB SOURCE_FILE ${SOURCE_FILES_DIR})
file(GLOB HEADER_FILE ${HEADER_FILES_DIR})
file(GLOB RESOURCE_FILE ${RESOURCE_FILES_DIR})
add_library(${PROJECT_NAME} ${SOURCE_FILE} ${HEADER_FILE} ${RESOURCE_FILE})

target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES}
                                      glfw)
