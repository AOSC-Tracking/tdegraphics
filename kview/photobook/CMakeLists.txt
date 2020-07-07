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
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


##### other data ################################

tde_install_icons( photobook )

tde_create_translated_desktop(
  SOURCE photobook.desktop
  DESTINATION ${SERVICES_INSTALL_DIR}
  PO_DIR kview-desktops
)

install( FILES photobookui.rc DESTINATION ${DATA_INSTALL_DIR}/photobook )


##### libphotobook (module) #####################

tde_add_kpart( libphotobook AUTOMOC
  SOURCES photobook.cpp
  LINK tdeparts-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)
