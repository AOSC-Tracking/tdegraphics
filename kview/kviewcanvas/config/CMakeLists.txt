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
    kviewcanvasconfig.desktop
  DESTINATION ${SERVICES_INSTALL_DIR}/kconfiguredialog )


##### kcm_kviewcanvasconfig (module) ############

tde_add_kpart( kcm_kviewcanvasconfig AUTOMOC
  SOURCES generalconfigwidget.ui confmodules.cpp
  LINK tdeui-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)
