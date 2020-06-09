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

tde_import( libkscan )

add_subdirectory( pics )

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/libkscan
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


##### other data ################################

tde_create_translated_desktop( kooka.desktop )
install( FILES kookarc DESTINATION ${CONFIG_INSTALL_DIR} )
install( FILES kookaui.rc DESTINATION ${DATA_INSTALL_DIR}/kooka )


##### kooka (executable) ########################

tde_add_executable( kooka AUTOMOC
  SOURCES
    main.cpp kooka.cpp kookaview.cpp kookapref.cpp img_saver.cpp
    ksaneocr.cpp kookaimage.cpp kookaimagemeta.cpp scanpackager.cpp
    imgnamecombo.cpp imageselectline.cpp thumbview.cpp thumbviewitem.cpp
    dwmenuaction.cpp kocrbase.cpp kocrgocr.cpp kocrkadmos.cpp
    kadmosocr.cpp ocrword.cpp ocrresedit.cpp kookaprint.cpp
    imgprintdialog.cpp kocrocrad.cpp
  LINK kscan-shared tdeparts-shared tdeprint-shared
  DESTINATION ${BIN_INSTALL_DIR}
)
