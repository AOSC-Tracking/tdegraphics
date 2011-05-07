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

install( FILES kfile_ps.desktop DESTINATION ${SERVICES_INSTALL_DIR} )


#### kfile_ps (module) ########################

tde_add_kpart( kfile_ps AUTOMOC
  SOURCES kfile_ps.cpp
  LINK dscparse-static kio-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)
