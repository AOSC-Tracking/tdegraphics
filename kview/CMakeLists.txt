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

add_subdirectory( kimageviewer )
add_subdirectory( config )
add_subdirectory( kviewcanvas )
add_subdirectory( kviewviewer )
add_subdirectory( modules )
add_subdirectory( photobook )

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

tde_install_icons( kview )

tde_create_translated_desktop(
  SOURCE kview.desktop
  PO_DIR kview-desktops
)

install( FILES kviewui.rc DESTINATION ${DATA_INSTALL_DIR}/kview )


##### kview (tdeinit) ###########################

tde_add_tdeinit_executable( kview AUTOMOC
  SOURCES kview.cpp main.cpp
  LINK tdeparts-shared tdeutils-shared
)
