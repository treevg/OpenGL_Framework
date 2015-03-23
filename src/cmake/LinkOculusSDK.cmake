find_package(OculusSDK)

include_directories(
	${OCULUS_SDK_INCLUDE_DIRS}
)

link_libraries(
	${OCULUS_SDK_LIBRARIES}
	ws2_32
	Dbghelp
	dxgi
	dxguid
	d3d10
	d3d11
	d3dcompiler
	winmm
	kernel32
	user32
	gdi32
	winspool
	comdlg32
	advapi32
	shell32
)