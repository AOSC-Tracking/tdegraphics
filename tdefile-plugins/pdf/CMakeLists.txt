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
  ${CMAKE_SOURCE_DIR}/tdefile-plugins/dependencies/poppler-tqt
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


#### other data #################################

install( FILES tdefile_pdf.desktop DESTINATION ${SERVICES_INSTALL_DIR} )


#### tdefile_pdf (module) #########################

tde_add_kpart( tdefile_pdf AUTOMOC
  SOURCES tdefile_pdf.cpp
  LINK tdeio-shared poppler-tqt-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)
