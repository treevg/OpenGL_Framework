cmake_minimum_required(VERSION 2.8)

find_package(DevIL REQUIRED)

include_directories(
	${IL_INCLUDE_PATHS}
)

set(ALL_LIBRARIES ${ALL_LIBRARIES}
	${IL_LIBRARIES}
    ${ILU_LIBRARIES}
    ${ILUT_LIBRARIES}
) 

add_definitions(-ILUT_USE_OPENGL)