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

add_subdirectory( pics )

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

tde_create_translated_desktop( kruler.desktop )

tde_create_translated_desktop(
  SOURCE eventsrc
  DESTINATION ${DATA_INSTALL_DIR}/kruler
  PO_DIR ${CMAKE_SOURCE_DIR}/translations/desktop_files/kruler-eventsrc
)

install( FILES move.wav DESTINATION ${DATA_INSTALL_DIR}/kruler/sounds )

install( FILES uninstall.desktop DESTINATION ${APPS_INSTALL_DIR}/Graphics RENAME kruler.desktop )


#### kruler (executable) ########################

tde_add_executable( kruler AUTOMOC
  SOURCES klineal.cpp main.cpp
  LINK tdeui-shared
  DESTINATION ${BIN_INSTALL_DIR}
)
