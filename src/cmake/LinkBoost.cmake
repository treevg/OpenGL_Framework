find_package(Boost COMPONENTS system)

include_directories(
	${Boost_INCLUDE_DIRS}
)

link_libraries(
	${Boost_LIBRARIES}
)