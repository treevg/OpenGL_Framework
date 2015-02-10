find_package(ASSIMP)

include_directories(
	${ASSIMP_INCLUDE_DIR}
)

target_link_libraries(
	${ProjectId}
	${ASSIMP_LIBRARY}
)