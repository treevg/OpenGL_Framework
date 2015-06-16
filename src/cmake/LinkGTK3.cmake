find_package(GTK3)   
   
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
   
