#############################################
# Try to find ASSMIP and set the following: #
#                                           #
# ASSIMP_FOUND                              #
# ASSIMP_INCLUDE_DIRS                       #
# ASSIMP_LIBRARIES                          #
#############################################

SET( DEVIL_SEARCH_PATHS
	$ENV{DEVIL_ROOT}
    ${DEVIL_ROOT_DIR}                  # DEVIL!
    ./lib/DevIL
    $ENV{PROGRAMFILES}/DEVIL           # WINDOWS
    ~/Library/Frameworks                # MAC
    /Library/Frameworks                 # MAC
    /usr/local                          # LINUX/MAC/UNIX
    /usr                                # LINUX/MAC/UNIX
    /opt                                # LINUX/MAC/UNIX
    /sw                                 # Fink
    /opt/local                          # DarwinPorts
    /opt/csw                            # Blastwave
)

FIND_PATH( DEVIL_INCLUDE_DIRS
    NAMES
        IL/il.h
    PATHS
        ${DEVIL_SEARCH_PATHS}
    PATH_SUFFIXES
        include
)

FIND_LIBRARY( DEVIL_LIBRARIES
    NAMES
        DevIL DEVIL
    PATHS
        ${DEVIL_SEARCH_PATHS}
    PATH_SUFFIXES
        lib
)

# Check if we found it!
IF ( DEVIL_INCLUDE_DIRS AND DEVIL_LIBRARIES )
    SET( DEVIL_FOUND TRUE )
    MESSAGE(STATUS "Looking for DevIL - found")
ELSE ( DEVIL_INCLUDE_DIRS AND ASSIMP_LIBRARIES )
    SET( DEVIL_FOUND FALSE )
    MESSAGE(STATUS "Looking for DevIL - not found")
ENDIF ( DEVIL_INCLUDE_DIRS AND DEVIL_LIBRARIES )