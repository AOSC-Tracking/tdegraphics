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
  ${CMAKE_BINARY_DIR}
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


##### other data ################################

tde_create_translated_desktop(
  SOURCE kpdf.desktop
  PO_DIR ${CMAKE_SOURCE_DIR}/translations/desktop_files/kpdf-desktops
)

install( FILES shell.rc DESTINATION ${DATA_INSTALL_DIR}/kpdf )


##### kpdf (executable) #########################

tde_add_executable( kpdf AUTOMOC
  SOURCES main.cpp shell.cpp
  LINK tdeparts-shared
  DESTINATION ${BIN_INSTALL_DIR}
)
