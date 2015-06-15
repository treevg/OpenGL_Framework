include_directories(
	${GLEW_INCLUDE_DIR}
)

link_libraries(
	${GLEW_LIBRARIES}
)

add_definitions(
	-DGLEW_STATIC
)