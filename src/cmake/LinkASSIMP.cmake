find_package(ASSIMP)

include_directories(
	${ASSIMP_INCLUDE_DIR}
)

link_libraries(
	${ASSIMP_LIBRARIES}
)