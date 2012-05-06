#################################################
#
#  (C) 2010-2011 Calvin Morrison
#  mutantturkey@gmail.com
#
#  Improvements and feedback are welcome
#
#  This file is released under GPL >= 2
#
#################################################

project( libpoppler-tqt )

##### configure checks ##########################

include( ConfigureChecks.cmake )

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_BINARY_DIR}
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/kfile-plugins/dependencies/poppler-tqt
  ${POPPLER_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)

##### headers ###################################

install( FILES
    poppler-qt.h poppler-page-transition.h poppler-link-qt3.h
  DESTINATION ${INCLUDE_INSTALL_DIR} )

##### poppler-tqt (shared) ############################

tde_add_library( poppler-tqt SHARED AUTOMOC
  SOURCES
    poppler-document.cc poppler-fontinfo.cc
    poppler-link.cc poppler-page.cc
    poppler-page-transition.cc poppler-page-transition-private.h
    poppler-private.cc poppler-private.h
  VERSION 0.0.0
  LINK poppler ${TQT_LIBRARIES}
  DESTINATION ${LIB_INSTALL_DIR}
)
