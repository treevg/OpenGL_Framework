cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0022 OLD)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

find_package(Bullet)
find_package(Assimp)

include_directories(
    ${OpenGL3_INCLUDE_PATH}
    ${GLEW_INCLUDE_PATH}
    ${GLFW3_INCLUDE_PATH}
    ${BULLET_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
    ${GLM_INCLUDE_PATH}
    ${EXTERNAL_LIBRARY_PATHS}
    ${LIBRARIES_PATH}
    ${CMAKE_SOURCE_DIR}/../../ovr_sdk_linux_0.5.0.1/LibOVR/Include
    ${CMAKE_SOURCE_DIR}/../../ovr_sdk_linux_0.5.0.1/LibOVRKernel/Src



)

file(GLOB_RECURSE SOURCES *.cpp)
file(GLOB_RECURSE HEADER *.h)

add_definitions(-DSHADERS_PATH="${SHADERS_PATH}")
add_definitions(-DRESOURCES_PATH="${RESOURCES_PATH}")
add_definitions(-DGLFW_INCLUDE_GLCOREARB)
add_definitions(-DGLEW_STATIC)

add_executable(${ProjectId} ${SOURCES} ${HEADER})

target_link_libraries(
    ${ProjectId}
    ${ALL_LIBRARIES}
    ${BULLET_LIBRARIES}
    ${ASSIMP_LIBRARIES}
    ${GLFW3_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${OpenGL3_LIBRARIES}
    ${CMAKE_SOURCE_DIR}/../../ovr_sdk_linux_0.5.0.1/LibOVR/Lib/Linux/x86_64/Debug/libOVRRT64_0.so.5
)
