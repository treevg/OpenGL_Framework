find_package(KinectSDK)

include_directories(
	${KINECT_SDK_INCLUDE_DIR}
)

link_libraries(
	${KINECT_SDK_LIBRARY}
)