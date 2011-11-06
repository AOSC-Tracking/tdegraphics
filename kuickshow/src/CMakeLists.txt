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
  ${IMLIB_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


##### other data ################################

tde_install_icons( kuickshow )
install( FILES kuickshow.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )


##### kuickshow (tdeinit) #######################

tde_add_tdeinit_executable( kuickshow AUTOMOC
  SOURCES
    kuickshow.cpp aboutwidget.cpp generalwidget.cpp kuickconfigdlg.cpp
    main.cpp defaultswidget.cpp imagewindow.cpp kuickdata.cpp
    imdata.cpp filefinder.cpp kurlwidget.cpp filewidget.cpp kuick.cpp
    imlibwidget.cpp slideshowwidget.cpp printing.cpp kuickfile.cpp
    kuickimage.cpp filecache.cpp
  LINK tdeprint-shared ${IMLIB_LIBRARIES}
)
