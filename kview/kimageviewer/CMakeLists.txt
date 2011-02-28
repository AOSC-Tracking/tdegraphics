#################################################
#
#  (C) 2010-2011 Serghei Amelian
#  serghei (DOT) amelian (AT) gmail.com
#
#  Improvements and feedback are welcome
#
#  This file is released under GPL >= 2
#
#################################################

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


##### other data ################################

install( FILES
    kimageviewercanvas.desktop kimageviewer.desktop
  DESTINATION ${SERVICETYPES_INSTALL_DIR} )


##### kimageviewer (shared) #####################

tde_add_library( kimageviewer SHARED AUTOMOC
  SOURCES canvas.cpp viewer.cpp
  VERSION 0.0.0
  LINK kparts-shared
  DESTINATION ${LIB_INSTALL_DIR}
)
