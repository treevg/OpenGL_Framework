include(FindPackageHandleStandardArgs)

find_path(GLM_INCLUDE_DIR glm/glm.hpp
  PATH_SUFFIXES include
)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLM DEFAULT_MSG GLM_INCLUDE_DIR)
