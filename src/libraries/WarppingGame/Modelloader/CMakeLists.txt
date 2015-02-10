cmake_minimum_required(VERSION 2.8)
include(${CMAKE_MODULE_PATH}/DefaultLibrary.cmake)

find_package(Assimp)

include_directories(

    ${ASSIMP_INCLUDE_DIRS}

)

target_link_libraries(
   
    ${ASSIMP_LIBRARIES}
 
)
