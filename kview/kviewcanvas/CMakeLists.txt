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
  SOURCE kviewcanvas.desktop
  DESTINATION ${SERVICES_INSTALL_DIR}
  PO_DIR ${CMAKE_SOURCE_DIR}/translations/desktop_files/kview-desktops
)


##### libkviewcanvas (module) ###################

tde_add_kpart( libkviewcanvas AUTOMOC
  SOURCES kimageholder.cpp kimagecanvas.cpp
  LINK kimageviewer-shared tdeutils-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)
