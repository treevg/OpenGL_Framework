find_package(LeapSDK)

include_directories(
	${LEAP_INCLUDE_DIR}
)

link_libraries(
	${LEAP_LIBRARY}
)