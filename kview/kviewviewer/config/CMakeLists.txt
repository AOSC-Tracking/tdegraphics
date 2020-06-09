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

tde_create_translated_desktop(
  SOURCE kviewviewerpluginsconfig.desktop
  DESTINATION ${SERVICES_INSTALL_DIR}/tdeconfiguredialog
  PO_DIR ${CMAKE_SOURCE_DIR}/translations/desktop_files/kview-desktops
)


##### kcm_kviewviewerpluginsconfig (module) #####

tde_add_kpart( kcm_kviewviewerpluginsconfig AUTOMOC
  SOURCES kviewviewerpluginsconfig.cpp
  LINK tdeutils-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)
