#find_package(GTK3)

IF (DEFINED ENV{GTK_ROOT})
    SET(GTK3_ROOT "$ENV{GTK_ROOT}")
ENDIF()
SET(GTK3_ROOT
    "${GTK3_ROOT}"
    CACHE
    PATH
    "Root directory to search for GTK3")

#IF(GTK3_FOUND)
FIND_PATH(GTK_INCLUDE_DIR 
	NAMES
		gtk-3.0/gtk/gtk.h
	 PATHS
		${GTK3_ROOT}
	 PATH_SUFFIXES
		include
	)
 
FIND_PATH(GTK_LIBRARY_DIR
	NAMES
		glib-2.0.lib
	 PATHS
		${GTK3_ROOT}
	 PATH_SUFFIXES
		lib
   )
   
    FIND_LIBRARY(GTK_ATK
        NAMES atk-1.0.lib
        PATHS ${GTK_LIBRARY_DIR}
    )
	FIND_LIBRARY(GTK_GDK_PIXBUF
        NAMES gdk_pixbuf-2.0.lib
        PATHS ${GTK_LIBRARY_DIR}
    )
	FIND_LIBRARY(GTK_PANGOWIN
        NAMES pangowin32-1.0.lib
        PATHS ${GTK_LIBRARY_DIR}
    )
	FIND_LIBRARY(GTK_PANGOCAIRO
        NAMES pangocairo-1.0.lib
        PATHS ${GTK_LIBRARY_DIR}
    )
	FIND_LIBRARY(GTK_PANGO
        NAMES pango-1.0.lib
        PATHS ${GTK_LIBRARY_DIR}
    )
	FIND_LIBRARY(GTK_CAIRO
        NAMES cairo.lib
        PATHS ${GTK_LIBRARY_DIR}
    )
	FIND_LIBRARY(GTK_GTHREAD
        NAMES gthread-2.0.lib
        PATHS ${GTK_LIBRARY_DIR}
    )
	FIND_LIBRARY(GTK_GOBJECT
        NAMES gobject-2.0.lib
        PATHS ${GTK_LIBRARY_DIR}
    )
	FIND_LIBRARY(GTK_GMODULE
        NAMES gmodule-2.0.lib
        PATHS ${GTK_LIBRARY_DIR}
    )
	FIND_LIBRARY(GTK_GLIB
        NAMES glib-2.0.lib
        PATHS ${GTK_LIBRARY_DIR}
    )
	FIND_LIBRARY(GTK_WIN32
        NAMES gtk-win32-3.0.lib
        PATHS ${GTK_LIBRARY_DIR}
    )
	
	SET(GTK3_LIBRARIES
	${GTK_ATK}
	${GTK_GDK_PIXBUF}
	${GTK_PANGOWIN}
	${GTK_PANGOCAIRO}
	${GTK_PANGO}
	${GTK_CAIRO}
	${GTK_GTHREAD}
	${GTK_GOBJECT}
	${GTK_GMODULE}
	${GTK_GLIB}
	${GTK_WIN32})
	
	include_directories(
   ${GTK_INCLUDE_DIR}
   ${GTK_INCLUDE_DIR}/gt3.0
   ${GTK_INCLUDE_DIR}/glib-2.0
   ${GTK_INCLUDE_DIR}/glib-2.0/include
   ${GTK3_ROOT}/lib/glib-2.0/include
   ${GTK_INCLUDE_DIR}/pango-1.0
   ${GTK_INCLUDE_DIR}/cairo
   ${GTK_INCLUDE_DIR}/gdk-pixbuff-2.0
   ${GTK_INCLUDE_DIR}/atk-1.0)
   

#ENDIF(GTK3_FOUND)

