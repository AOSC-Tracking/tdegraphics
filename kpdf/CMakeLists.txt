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

add_subdirectory( xpdf )
add_subdirectory( conf )
add_subdirectory( core )
add_subdirectory( ui )
add_subdirectory( shell )

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/xpdf
  ${CMAKE_CURRENT_SOURCE_DIR}/xpdf/goo
  ${CMAKE_BINARY_DIR}
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


##### other data ################################

tde_install_icons( kpdf )
install( FILES kpdf_part.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
install( FILES part.rc DESTINATION ${DATA_INSTALL_DIR}/kpdfpart )


##### libkpdfpart (module) ######################

tde_add_kpart( libkpdfpart AUTOMOC
  SOURCES
    dcop.skel error.cpp part.cpp
  LINK
    kpdfconf-static kpdfui-static kpdfcore-static generatorkimgio-static
    generatorpdf-static xpdf-static goo-static splash-static fofi-static
    tdeutils-shared tdeprint-shared tdeparts-shared ${PAPER_LIBRARY} ${XFT_LIBRARIES}
  DEPENDENCIES kpdfconf-static
  DESTINATION ${PLUGIN_INSTALL_DIR}
)
