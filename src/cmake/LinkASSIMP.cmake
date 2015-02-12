find_package(ASSIMP)

include_directories(
	${ASSIMP_INCLUDE_DIRS}
)

link_libraries(
	${ASSIMP_LIBRARIES}
)