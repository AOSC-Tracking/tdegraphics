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

tde_import( kviewshell )

add_subdirectory( libkfaximage )

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/libkfaximage
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/kviewshell
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


#### other data #################################

tde_install_icons( kfaxview )

tde_create_translated_desktop(
  SOURCE kfaxview.desktop
  PO_DIR ${CMAKE_SOURCE_DIR}/translations/desktop_files/kfaxview-desktops
)

install( FILES ${CMAKE_SOURCE_DIR}/kviewshell/kviewshell.rc DESTINATION ${DATA_INSTALL_DIR}/kfaxview/kviewshell.rc )

foreach( _desktop kfaxmultipage.desktop kfaxmultipage_tiff.desktop )
  tde_create_translated_desktop(
    SOURCE ${_desktop}
    DESTINATION ${SERVICES_INSTALL_DIR}
    PO_DIR ${CMAKE_SOURCE_DIR}/translations/desktop_files/kfaxview-desktops
  )
endforeach()


#### kfaxviewpart (module) ######################

tde_add_kpart( kfaxviewpart AUTOMOC
  SOURCES faxmultipage.cpp faxrenderer.cpp
  LINK tdemultipage-shared kfaximage-static tdeparts-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)


#### kfaxview (executable) ######################

tde_add_executable( kfaxview AUTOMOC
  SOURCES main.cpp
  LINK kviewshell-static tdeio-shared tdeparts-shared
  DESTINATION ${BIN_INSTALL_DIR}
)
