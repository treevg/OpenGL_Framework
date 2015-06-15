include_directories(
	${GLFW3_INCLUDE_DIR}
)

link_libraries(
	${GLFW3_LIBRARIES}
)

add_definitions(
	-DGLFW_INCLUDE_GLCOREARB
)