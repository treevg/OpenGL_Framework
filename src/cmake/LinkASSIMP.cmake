cmake_minimum_required(VERSION 2.8)

find_package(DevIL REQUIRED)

include_directories(
	${ASSIMP_INCLUDE_DIR}
)

set(ALL_LIBRARIES ${ALL_LIBRARIES}
	${ASSIMP_LIBRARY}
)