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

add_subdirectory( plugins )

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


##### other data ################################

tde_create_translated_desktop(
  SOURCE kviewgeneralconfig.desktop
  DESTINATION ${SERVICES_INSTALL_DIR}/tdeconfiguredialog
  PO_DIR kview-desktops
)

install( FILES kview.setdlg DESTINATION ${DATA_INSTALL_DIR}/kview )


##### kcm_kviewgeneralconfig (module) ###########

tde_add_kpart( kcm_kviewgeneralconfig AUTOMOC
  SOURCES kviewconfmodules.cpp
  LINK tdeui-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)
