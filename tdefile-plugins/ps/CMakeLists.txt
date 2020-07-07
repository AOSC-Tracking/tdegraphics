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
  ${CMAKE_SOURCE_DIR}/kghostview
  ${CMAKE_SOURCE_DIR}/kghostview/dscparse
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


#### other data #################################

tde_create_translated_desktop(
  SOURCE tdefile_ps.desktop gsthumbnail.desktop
  DESTINATION ${SERVICES_INSTALL_DIR}
  PO_DIR tdefile-desktops
)


#### tdefile_ps (module) ########################

tde_add_kpart( tdefile_ps AUTOMOC
  SOURCES tdefile_ps.cpp
  LINK dscparse-static tdeio-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)


##### gsthumbnail (module) ######################

tde_add_kpart( gsthumbnail AUTOMOC
  SOURCES gscreator.cpp
  LINK dscparse-static ${TQT_LIBRARIES}
  DESTINATION ${PLUGIN_INSTALL_DIR}
)
