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
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/kview
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


##### other data ################################

tde_create_translated_desktop(
  SOURCE kviewpresenter.desktop
  DESTINATION ${DATA_INSTALL_DIR}/kview/kpartplugins
  PO_DIR ${CMAKE_SOURCE_DIR}/translations/desktop_files/kview-desktops
)

install(
  FILES
    kviewpresenter.rc
  DESTINATION ${DATA_INSTALL_DIR}/kview/kpartplugins
)


##### kview_presenterplugin (module) ############

tde_add_kpart( kview_presenterplugin AUTOMOC
  SOURCES
    imagelistitem.cpp imagelistdialog.ui kviewpresenter.cpp
  LINK tdeio-shared tdeparts-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)
