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

FIND_PATH( IL_INCLUDE_DIR
    NAMES
        IL/il.h
    PATHS
        ${DEVIL_SEARCH_PATHS}
    PATH_SUFFIXES
        include
)

FIND_LIBRARY( IL_LIBRARIES
    NAMES
        DevIL DEVIL
    PATHS
        ${DEVIL_SEARCH_PATHS}
    PATH_SUFFIXES
        lib
)
FIND_LIBRARY( ILU_LIBRARIES
    NAMES
        ILU
    PATHS
        ${DEVIL_SEARCH_PATHS}
    PATH_SUFFIXES
        lib
)
FIND_LIBRARY( ILUT_LIBRARIES
    NAMES
        ILUT
    PATHS
        ${DEVIL_SEARCH_PATHS}
    PATH_SUFFIXES
        lib
)


 SET( DEVIL_FOUND TRUE )
