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

install( FILES kfile_png.desktop DESTINATION ${SERVICES_INSTALL_DIR} )


#### kfile_png (module) #########################
tde_add_kpart( kfile_png AUTOMOC
  SOURCES kfile_png.cpp
  LINK kio-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)
