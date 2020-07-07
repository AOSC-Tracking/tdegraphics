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

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/kviewshell
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


#### other data #################################

tde_create_translated_desktop(
  SOURCE kdvi.desktop
  PO_DIR kdvi-desktops
)

tde_create_translated_desktop(
  SOURCE kdvimultipage.desktop
  DESTINATION ${SERVICES_INSTALL_DIR}
  PO_DIR kdvi-desktops
)

install( FILES
    kdvi.kcfg
  DESTINATION ${KCFG_INSTALL_DIR} )

install( FILES
    tips kdvi_part.rc
    ${CMAKE_SOURCE_DIR}/kviewshell/kviewshell.rc
  DESTINATION ${DATA_INSTALL_DIR}/kdvi )


#### squeeze psheader.txt #######################

tde_add_executable( squeeze
  SOURCES squeeze.c
)

add_custom_command(
  OUTPUT psheader.c
  COMMAND squeeze ${CMAKE_CURRENT_SOURCE_DIR}/psheader.txt psheader.c
  DEPENDS psheader.txt squeeze
)


#### kdvipart (module) ##########################

tde_add_kpart( kdvipart AUTOMOC
  SOURCES
    renderedDviPagePixmap.cpp dviPageCache.cpp
    kdvi_multipage.cpp kdvi_multipage_texthandling.cpp dviRenderer.cpp
    bigEndianByteReader.cpp infodialog.cpp psheader.c
    dviRenderer_draw.cpp dviRenderer_prescan.cpp
    dviRenderer_export.cpp dviFile.cpp fontpool.cpp fontprogress.cpp
    psgs.cpp fontMap.cpp fontEncoding.cpp fontEncodingPool.cpp
    special.cpp util.cpp vf.cpp glyph.cpp optionDialogFontsWidget.cpp
    optionDialogFontsWidget_base.ui optionDialogSpecialWidget.cpp
    optionDialogSpecialWidget_base.ui TeXFont.cpp TeXFont_PK.cpp
    TeXFont_PFB.cpp TeXFont_TFM.cpp TeXFontDefinition.cpp
    dviWidget.cpp dvisourcesplitter.cpp prefs.kcfgc
  LINK
    tdemultipage-shared tdeprint-shared tdeparts-shared ${FREETYPE_LIBRARIES}
  DESTINATION ${PLUGIN_INSTALL_DIR}
)


##### kdvi (executable) ####################

tde_add_executable( kdvi AUTOMOC
  SOURCES main.cpp
  LINK kviewshell-static tdeparts-shared
  DESTINATION ${BIN_INSTALL_DIR}
)

add_subdirectory( pix )