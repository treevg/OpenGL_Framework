cmake_minimum_required(VERSION 2.8)
get_filename_component(FolderId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/${FolderId}/)
GENERATE_SUBDIRS(ALL_EXECUTABLES ${EXECUTABLES_PATH}/${FolderId} ${PROJECT_BINARY_DIR}/executables/${FolderId})
