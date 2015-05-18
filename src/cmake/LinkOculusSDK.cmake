find_package(OculusSDK)

include_directories(
	${OCULUS_SDK_INCLUDE_DIRS}
)

link_libraries(
	${OCULUS_SDK_LIBRARIES}

)
