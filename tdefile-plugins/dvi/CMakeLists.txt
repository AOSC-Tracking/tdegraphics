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

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_BINARY_DIR}
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


#### other data #################################

tde_create_translated_desktop(
  SOURCE tdefile_dvi.desktop
  DESTINATION ${SERVICES_INSTALL_DIR}
  PO_DIR ${CMAKE_SOURCE_DIR}/translations/desktop_files/tdefile-desktops
)


#### tdefile_dvi (module) #########################

tde_add_kpart( tdefile_dvi AUTOMOC
  SOURCES tdefile_dvi.cpp
  LINK tdeio-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)
