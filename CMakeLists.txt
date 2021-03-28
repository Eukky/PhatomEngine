cmake_minimum_required(VERSION 3.0.0)
project(Phatom VERSION 0.1.0)

# include(CTest)
# enable_testing()

set(CMAKE_CXX_STANDARD 20)

set(CORE_MODULE_NAME "Core")

set(INCLUDE_DIR ${CORE_MODULE_NAME}/include)
set(HEADER_FILES_DIR ${CORE_MODULE_NAME}/include/*.h)
set(SOURCE_FILES_DIR ${CORE_MODULE_NAME}/src/*.cpp)

include_directories("/usr/local/include")
include_directories("./ThirdParty/stb")
include_directories("./ThirdParty/tiny_obj_loader")

include_directories(${INCLUDE_DIR})
file(GLOB_RECURSE SOURCE_FILE ${SOURCE_FILES_DIR})
file(GLOB_RECURSE HEADER_FILE ${HEADER_FILES_DIR})
add_executable(Phatom ${SOURCE_FILE} ${HEADER_FILE})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

find_package(Vulkan REQUIRED FATAL_ERROR)
include_directories(${Vulkan_INCLUDE_DIRS})
target_link_libraries(Phatom ${Vulkan_LIBRARIES})

find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
target_link_libraries(Phatom glfw)