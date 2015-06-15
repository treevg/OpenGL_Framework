include(FindPackageHandleStandardArgs)

find_path(ASSIMP_INCLUDE_DIR assimp/mesh.h
  PATH_SUFFIXES include
)

find_library(ASSIMP_LIBRARIES
  NAMES assimp
  PATH_SUFFIXES lib
)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(ASSIMP DEFAULT_MSG
                                  ASSIMP_LIBRARIES ASSIMP_INCLUDE_DIR)