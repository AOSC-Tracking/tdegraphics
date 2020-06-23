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

add_subdirectory( backends )

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../core
  ${CMAKE_CURRENT_SOURCE_DIR}/../dom
  ${CMAKE_CURRENT_SOURCE_DIR}/../ecma
  ${CMAKE_CURRENT_SOURCE_DIR}/../impl
  ${CMAKE_CURRENT_SOURCE_DIR}/../impl/libs/libtext2path/src
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
  ${ART_LIBRARY_DIRS}
)


##### headers ###################################

install( FILES
    ksvg_plugin.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/ksvg )


##### other data ################################

foreach( _service ksvgplugin.desktop svgthumbnail.desktop )
  tde_create_translated_desktop(
    SOURCE ${_service}
    DESTINATION ${SERVICES_INSTALL_DIR}
    PO_DIR ${CMAKE_SOURCE_DIR}/translations/desktop_files/ksvg-desktops
  )
endforeach()

install( FILES
    ksvgplugin.rc
  DESTINATION ${DATA_INSTALL_DIR}/ksvg )


##### libksvgplugin (module) ####################

tde_add_kpart( libksvgplugin AUTOMOC
  SOURCES
    ksvg_plugin.cpp ksvg_factory.cpp ksvg_widget.cpp
  LINK ksvg-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)


##### svgthumbnail (module) #####################

tde_add_kpart( svgthumbnail AUTOMOC
  SOURCES svgcreator.cpp
  LINK ksvg-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)
