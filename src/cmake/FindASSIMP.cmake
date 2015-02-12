#
# Try to find GLFW3 library and include path.
# Once done this will define
#
# ASSIMP_FOUND
# ASSIMP_INCLUDE_PATH
# ASSIMP_LIBRARY

set(ASSIMP_ENV $ENV{ASSIMP_ROOT})

#IF (MINGW)
#    FIND_PATH( ASSIMP_INCLUDE_PATH GLFW/glfw3.h
#        ${GLFW3_ROOT_ENV}/include
#    )
#
#    FIND_LIBRARY( ASSIMP_LIBRARY
#        NAMES assimp
#        PATHS
#        ${GLFW3_ROOT_ENV}/lib-mingw
#    )
#
#ELSEIF (MSVC)
#    FIND_PATH( ASSIMP_INCLUDE_PATH GLFW/glfw3.h
#        ${GLFW3_ROOT_ENV}/include
#    )
#
#    IF (MSVC10)
#        FIND_LIBRARY( ASSIMP_LIBRARY
#            NAMES assimp
#            PATHS
#            ${GLFW3_ROOT_ENV}/lib-msvc100
#        )
#    ELSE()
#        FIND_LIBRARY( ASSIMP_LIBRARY
#            NAMES assimp
#            PATHS
#            ${GLFW3_ROOT_ENV}/lib-msvc110
#        )
#    ENDIF ()
#ELSEIF(APPLE)
#
#    FIND_PATH(ASSIMP_INCLUDE_PATH assimp/assimp.hpp DOC "Path to GLFW include directory."
#    HINTS ${GLFW3_ROOT_ENV}/include
#    PATHS /usr/include /usr/local/include /opt/local/include
#    )
#    
#    FIND_LIBRARY( ASSIMP_LIBRARY
#        NAMES libassimp.a assimp
#        PATHS $ENV{GLFW3_ROOT_ENV}/build/src /usr/lib /usr/local/lib /opt/local/lib
#    )
#
#ELSE()
    FIND_PATH(ASSIMP_INCLUDE_PATH assimp/ PATHS
        /usr/include
        /usr/local/include
        /opt/local/include)
    FIND_LIBRARY(ASSIMP_LIBRARY
        NAMES assimp libassimp.so
        PATHS /usr/lib/)
#ENDIF ()


SET(ASSIMP_FOUND "NO")
IF (ASSIMP_INCLUDE_PATH AND ASSIMP_LIBRARY)
    SET(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY})
    SET(ASSIMP_FOUND "YES")
    message("EXTERNAL LIBRARY 'ASSIMP' FOUND")
ELSE()
    message("ERROR: EXTERNAL LIBRARY 'ASSIMP' NOT FOUND")
ENDIF (ASSIMP_INCLUDE_PATH AND ASSIMP_LIBRARY)
