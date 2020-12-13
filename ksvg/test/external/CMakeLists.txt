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

# FIXME KDE_CXXFLAGS = $(USE_EXCEPTIONS)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../core
  ${CMAKE_CURRENT_SOURCE_DIR}/../../dom
  ${CMAKE_CURRENT_SOURCE_DIR}/../../ecma
  ${CMAKE_CURRENT_SOURCE_DIR}/../../impl
  ${CMAKE_CURRENT_SOURCE_DIR}/../../impl/libs/libtext2path/src
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
  ${ART_LIBRARY_DIRS}
)


##### svgdisplay (executable) ###################

tde_add_executable( svgdisplay AUTOMOC
  SOURCES SVGTestWidget.cpp svgdisplay.cpp
  LINK ksvg-shared
  DESTINATION ${BIN_INSTALL_DIR}
)


##### printnodetest (executable) ################

tde_add_executable( printnodetest AUTOMOC
  SOURCES printnodetest.cpp
  LINK ksvg-shared
  DESTINATION ${BIN_INSTALL_DIR}
)
