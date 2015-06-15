include(FindPackageHandleStandardArgs)

IF (WIN32)
  FIND_PATH( OpenGL3_INCLUDE_DIR 
    NAMES GL/glcorearb.h
    PATH_SUFFIXES include
  )
  SET(OpenGL3_LIBRARIES OpenGL32)

ELSEIF (APPLE)
 FIND_PATH(OpenGL3_INCLUDE_DIR OpenGL/gl3.h 
   OpenGL_ROOT_ENV/OpenGL/)
 SET(OpenGL3_LIBRARIES "-framework Cocoa -framework OpenGL -framework IOKit" CACHE STRING "OpenGL lib for OSX")
 
ELSE()

SET(OpenGL3_LIBRARIES "GL" CACHE STRING "OpenGL lib for Linux")
    FIND_PATH(OpenGL3_INCLUDE_DIR GL/gl.h
      /usr/share/doc/NVIDIA_GLX-1.0/include
      /usr/openwin/share/include
      /opt/graphics/OpenGL/include /usr/X11R6/include
    )
ENDIF ()

FIND_PACKAGE_HANDLE_STANDARD_ARGS(OpenGL3 DEFAULT_MSG
                                  OpenGL3_LIBRARIES OpenGL3_INCLUDE_DIR)