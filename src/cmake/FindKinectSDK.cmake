#SET(KINECTSDK20_DIR_DIR "c:/Program Files/Microsoft SDKs/Kinect/v1.0/")
IF(DEFINED ENV{KINECTSDK20_DIR})
SET(KINECTSDK20_DIR "$ENV{KINECTSDK20_DIR}")
ENDIF()
SET(KINECTSDK20_DIR
		"${KINECTSDK20_DIR}"
		CACHE
		PATH
		"Root directory to search for KinectSDK")

#message(${KINECTSDK20_DIR_DIR})
#SET(KINECT_SDK_LIB_DIR "${KINECTSDK20_DIR_DIR}/Lib/x86/")
FIND_PATH(KINECT_SDK_INCLUDE_DIR Kinect.h 
	${KINECTSDK20_DIR}/inc
	${KINECTSDK20_DIR}
	)

	
	SET(KINECT_SDK_DLLS 
	$ENV{SystemRoot}System32/Kinect20.dll
)

# IF (DEFINED ENV{KINECTSDK20_DIR_DIR})
   # SET(KINECTSDK20_DIR_DIR "${KINECTSDK20_DIR}")
# ENDIF()
# SET(KINECTSDK20_DIR
    # "${KINECTSDK20_DIR}"
    # "Root directory to search for KinectSDK")


SET(KINECT_SDK_INCLUDES 
	${KINECTSDK20_DIR}/inc

)
FIND_LIBRARY(KINECT_SDK_LIBRARY 
	NAMES Kinect20
	PATHS ${KINECTSDK20_DIR}/Lib/x86
)

#MARK_AS_ADVANCED(KINECT_SDK_LIBRARY)
# MARK_AS_ADVANCED(KINECT_SDK_LIBRARY_DEBUG)


# ####################   Macro   #######################
# MACRO(CHECK_FILES _FILES _DIR)
	# SET(_MISSING_FILES)
	# FOREACH(_FILE ${${_FILES}})
		# IF(NOT EXISTS "${_FILE}")
			# SET(KINECT_SDK_FOUND NO)
			# get_filename_component(_FILE ${_FILE} NAME)
			# SET(_MISSING_FILES "${_MISSING_FILES}${_FILE}, ")
		# ENDIF()
	# ENDFOREACH()
	# IF(_MISSING_FILES)
		# MESSAGE(STATUS "In folder \"${${_DIR}}\" not found files: ${_MISSING_FILES}")
		# SET(KINECT_SDK_FOUND NO)
	# ENDIF()
# ENDMACRO(CHECK_FILES)

# MACRO(CHECK_DIR _DIR)
	# IF(NOT EXISTS "${${_DIR}}")
		# MESSAGE(STATUS "Folder \"${${_DIR}}\" not found.")
		# SET(KINECT_SDK_FOUND NO)
	# ENDIF()
# ENDMACRO(CHECK_DIR)

# ##################### Checking #######################
# MESSAGE(STATUS "Searching KINECT_SDK.")
# SET(KINECT_SDK_FOUND YES)

# CHECK_DIR(KINECTSDK20_DIR_DIR)
# IF(KINECT_SDK_FOUND)
	# CHECK_DIR(KINECT_SDK_LIB_DIR)
	# CHECK_DIR(KINECT_SDK_INCLUDE_DIR)
	
	# IF(KINECT_SDK_FOUND)
		# #CHECK_FILES(KINECT_SDK_DLLS KINECTSDK20_DIR_DIR)
		# CHECK_FILES(KINECT_SDK_LIBRARIES KINECT_SDK_LIB_DIR)
		# CHECK_FILES(KINECT_SDK_INCLUDES KINECT_SDK_INCLUDE_DIR)
	# ENDIF()
# ENDIF()

# MESSAGE(STATUS "KINECT_SDK_FOUND - ${KINECT_SDK_FOUND}.")