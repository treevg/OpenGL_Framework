cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0022 OLD)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

include_directories(
    ${OpenGL3_INCLUDE_PATH}
    ${GLEW_INCLUDE_PATH}
    ${GLFW3_INCLUDE_PATH}
    ${GLM_INCLUDE_PATH}
    ${EXTERNAL_LIBRARY_PATHS}
    ${LIBRARIES_PATH}
    /usr/include/gstreamer-1.0/
    /usr/include/glib-2.0/
    /usr/lib/glib-2.0/include/
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
    ${GLFW3_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${OpenGL3_LIBRARIES}
    gstreamer-1.0
    gobject-2.0
    glib-2.0
)