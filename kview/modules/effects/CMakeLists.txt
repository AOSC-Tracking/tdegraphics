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
  ${CMAKE_SOURCE_DIR}/kview
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


##### other data ################################

install( FILES
    kvieweffects.desktop kvieweffects.rc
  DESTINATION ${DATA_INSTALL_DIR}/kview/kpartplugins )


##### kview_effectsplugin (module) ##############

tde_add_kpart( kview_effectsplugin AUTOMOC
  SOURCES kvieweffects.cpp
  LINK kparts-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)
