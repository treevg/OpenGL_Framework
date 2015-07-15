cmake_minimum_required(VERSION 2.8)
include(${CMAKE_MODULE_PATH}/DefaultExecutable.cmake)

find_package(VTK REQUIRED)
if(VTK_LIBRARIES)
        message ("VTK FOUND")
        include_directories(${VTK_INCLUDE_DIRS})
endif()
target_link_libraries(${ProjectId} ${VTK_LIBRARIES})
