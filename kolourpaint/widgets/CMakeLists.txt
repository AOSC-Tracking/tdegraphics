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
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/..
  ${CMAKE_CURRENT_SOURCE_DIR}/../pixmapfx
  ${CMAKE_CURRENT_SOURCE_DIR}/../tools
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)


##### kolourpaintwidgets (static) ###############

tde_add_library( kolourpaintwidgets STATIC_PIC AUTOMOC
  SOURCES
    kpcolorsimilaritycube.cpp kpcolorsimilaritydialog.cpp
    kpcolortoolbar.cpp kpresizesignallinglabel.cpp
    kpsqueezedtextlabel.cpp kptooltoolbar.cpp kptoolwidgetbase.cpp
    kptoolwidgetbrush.cpp kptoolwidgeterasersize.cpp
    kptoolwidgetfillstyle.cpp kptoolwidgetlinewidth.cpp
    kptoolwidgetopaqueortransparent.cpp kptoolwidgetspraycansize.cpp
)
