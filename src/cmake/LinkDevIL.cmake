find_package(DevIL)

include_directories(
	${IL_INCLUDE_DIR}
)

link_libraries(
	${IL_LIBRARIES} ${ILU_LIBRARIES} ${ILUT_LIBRARIES}
)

add_definitions(
	-ILUT_USE_OPENGL
)