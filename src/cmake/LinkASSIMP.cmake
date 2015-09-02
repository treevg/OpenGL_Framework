find_package(ASSIMP)

include_directories(
	${ASSIMP_INCLUDE_PATH}
)

link_libraries(
	${ASSIMP_LIBRARIES}
)