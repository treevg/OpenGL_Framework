find_package(DevIL)

include_directories(
	${IL_INCLUDE_DIR}
)

target_link_libraries(
	${ProjectId}
	${IL_LIBRARIES} ${ILU_LIBRARIES} ${ILUT_LIBRARIES}
)

add_definitions(
	-ILUT_USE_OPENGL
)