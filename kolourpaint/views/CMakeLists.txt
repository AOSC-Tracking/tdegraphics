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
  ${CMAKE_CURRENT_SOURCE_DIR}/..
  ${CMAKE_CURRENT_SOURCE_DIR}/../pixmapfx
  ${CMAKE_CURRENT_SOURCE_DIR}/../views
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)


##### kolourpaintviews (static) #################

tde_add_library( kolourpaintviews STATIC_PIC AUTOMOC
  SOURCES
    kpthumbnailview.cpp kpunzoomedthumbnailview.cpp
    kpzoomedthumbnailview.cpp kpzoomedview.cpp
)
