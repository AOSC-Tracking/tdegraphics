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

include( ConfigureChecks.cmake )

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_BINARY_DIR}
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
  ${OPENEXR_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


#### other data #################################

install( FILES tdefile_exr.desktop DESTINATION ${SERVICES_INSTALL_DIR} )


#### tdefile_exr (module) #########################

tde_add_kpart( tdefile_exr AUTOMOC
  SOURCES tdefile_exr.cpp
  LINK tdeio-shared ${OPENEXR_LIBRARIES}
  DESTINATION ${PLUGIN_INSTALL_DIR}
)
