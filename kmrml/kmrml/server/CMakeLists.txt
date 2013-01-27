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
  ${CMAKE_CURRENT_SOURCE_DIR}/lib
  ${CMAKE_BINARY_DIR}
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)

#### other data #################################

INSTALL( FILES daemonwatcher.desktop DESTINATION ${SERVICES_INSTALL_DIR}/kded )


#### kded_daemonwatcher (module) ################

tde_add_kpart( kded_daemonwatcher AUTOMOC
  SOURCES watcher.cpp watcher.skel
  LINK kmrmlstuff-static tdeparts-shared tdeio-shared tdeinit_kded-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)
