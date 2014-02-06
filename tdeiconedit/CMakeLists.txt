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

add_subdirectory ( pics )

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

install( FILES tdeiconedit.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES tdeiconeditui.rc DESTINATION ${DATA_INSTALL_DIR}/tdeiconedit )


#### tdeiconedit (executable) #####################

tde_add_executable( tdeiconedit AUTOMOC
  SOURCES
    utils.cpp main.cpp tdeiconedit.cpp tdeicongrid.cpp
    tdeiconcolors.cpp kcolorgrid.cpp palettetoolbar.cpp
    tdeicon.cpp kresize.cpp knew.cpp properties.cpp
    tdeiconeditslots.cpp tdeiconconfig.cpp
  LINK tdeio-shared tdeprint-shared
  DESTINATION ${BIN_INSTALL_DIR}
)
