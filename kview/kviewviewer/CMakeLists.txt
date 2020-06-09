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

add_subdirectory( config )

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/kview
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


##### other data ################################

tde_create_translated_desktop(
  SOURCE kviewviewer.desktop
  DESTINATION ${SERVICES_INSTALL_DIR}
  PO_DIR ${CMAKE_SOURCE_DIR}/translations/desktop_files/kview-desktops
)

install( FILES kviewviewer.rc kviewviewer_ro.rc kviewpopup.rc DESTINATION ${DATA_INSTALL_DIR}/kviewviewer )


##### libkviewviewer (module) ###################

tde_add_kpart( libkviewviewer AUTOMOC
  SOURCES
    printimagesettings.ui imagesettings.cpp
    kviewkonqextension.cpp kviewviewer.cpp
  LINK kimageviewer-shared tdeutils-shared tdeprint-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)
