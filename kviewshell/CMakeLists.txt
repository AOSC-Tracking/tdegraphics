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

project( kviewshell )

add_subdirectory( pics )
add_subdirectory( plugins )

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


##### headers ###################################

install( FILES
    kmultipageInterface.h
  DESTINATION ${INCLUDE_INSTALL_DIR} )

install( FILES
    anchor.h bookmark.h pageView.h documentPageCache.h
    documentRenderer.h documentWidget.h history.h hyperlink.h
    kmultipage.h length.h pageNumber.h pageSize.h renderedDocumentPage.h
    renderedDocumentPagePixmap.h renderedDocumentPagePrinter.h
    selection.h simplePageSize.h textBox.h zoom.h zoomlimits.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/kviewshell )


##### other data ################################

install( FILES kviewshell.rc DESTINATION ${DATA_INSTALL_DIR}/kviewshell )
install( FILES kviewerpart.rc DESTINATION ${DATA_INSTALL_DIR}/kviewerpart )
install( FILES kmultipage.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR} )
install( FILES emptymultipage.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
install( FILES kviewshell.kcfg DESTINATION ${KCFG_INSTALL_DIR} )


##### ifaces (static) ###########################

tde_add_library( ifaces STATIC_PIC AUTOMOC
  SOURCES
    kviewpart_iface.cpp
)


##### kviewshell (static) #######################

tde_add_library( kviewshell STATIC_PIC AUTOMOC
  SOURCES
    kviewshell.cpp
)


##### kmultipage (shared) #######################

tde_add_library( kmultipage SHARED AUTOMOC
  SOURCES
    kmultipage.cpp pageView.cpp marklist.cpp kmultipageInterface.skel
    units.cpp pageSize.cpp simplePageSize.cpp renderedDocumentPage.cpp
    renderedDocumentPagePixmap.cpp renderedDocumentPagePrinter.cpp
    documentPageCache.cpp documentWidget.cpp searchWidget.cpp
    selection.cpp documentRenderer.cpp history.cpp kvsprefs.kcfgc
    tableOfContents.cpp kprintDialogPage_pageoptions.cpp
  VERSION 0.0.0
  LINK kparts-shared tdeprint-shared
  DESTINATION ${LIB_INSTALL_DIR}
)


##### kviewerpart (module) ######################

tde_add_kpart( kviewerpart AUTOMOC
  SOURCES
    kviewpart.cpp pageSizeWidget_base.ui pageSizeWidget.cpp
    pageSizeDialog.cpp pageSize.cpp sizePreview.cpp zoom.cpp
    units.cpp kvsprefs.kcfgc optionDialogGUIWidget_base.ui
    optionDialogAccessibilityWidget.ui simplePageSize.cpp
  LINK ifaces-static kparts-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)


##### kviewshell (executable) ##################

tde_add_executable( kviewshell AUTOMOC
  SOURCES
    main.cpp
  LINK kviewshell-static kparts-shared
  DESTINATION ${BIN_INSTALL_DIR}
)


##### emptymultipagepart (module) ###############

tde_add_kpart( emptymultipagepart AUTOMOC
  SOURCES
    empty_multipage.cpp emptyRenderer.cpp
  LINK kmultipage-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)


##### install import cmake modules ###############

tde_install_export( )
